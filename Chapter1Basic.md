# Chapter 1 Basics

## Item 1: Distinguish between pointers and references
  * Both pointers and references let you refer to other objects indirectly.
  * Pointers:
    * If it is possible that there might not be an object to refer to, make the variable a pointer so that you can set it to be `null`.
    * So please do test a pointer against `null` before using it.
    * Pointers can be reassigned to refer to different objects.
  * References:
    * If the variable must always refer to an object, make the variable a reference.
    * So a reference must be initialized with a object value and there is no need to test validity of a reference.
    * A reference always refers to the object with which it is initialized.
    * We always want `operator[]` to return a reference.
  * Dereferencing a null pointer (something link `char *pc = 0; char &rc = *pc;`) is an undefined behavior.

## Item 2: Prefer C++-Style casts
  * Why is C-Style cast not good? (1), it will be nice to be able to specify more precisely the purpose of each cast; (2), C-Style casts are hard to find.
  * `static_cast<T>` has the same power and meaning as the general-purpose C-style cast.
  * `const_cast<T>` changes the constness or volatileness of something. But it should not modify anything other than constness or volatileness (For example cast down the inheritance hierarchy).
  * `dynamic_cast<T>` is used to perform safe casts down or across a inheritance hierarchy. This means that we cast base class objects into pointers or references to derived or sibling class.
    * If `dynamic_cast<T>` fails, pointer dynamic casting will lead to a `null` pointer, and reference dynamic casting will throw an exception.
    * `dynamic_cast<T>` cannot be applied to type lacking virtual functions, for example 
    ```C++
    int a = 5; dynamic_cast<double>(a);
    ```
    will lead to a compilation failure. In this case, please use `static_cast<T>` instead.
  * In the book, Scott implies that the last type of C++ style cast `reinterpret_cast<T>` is not portable, it is most commonly used to cast between function pointer types. So we will not go into details about this type of cast.
  * For sample codes of `const_cast<T>` and `dynamic_cast<T>`, please refer to [CppCasts.cpp](./SampleCodes/Chapter1Basic/CppCasts.cpp)

## Item 3: Never treat arrays polymorphically
  * Definition of Polymorphism: Manipulate derived through pointers and references to base class objects.
  * Polymorphism and pointer arithmetic don't mix for the following reasons.
    In this part, we first define the following base and derived classes.
      ```C++
      class BST { ... };
      class BalancedBST : public BST { ... };
      ```
    ### Problem 1: Assign derived class objects to base class arrays
      Consider the following function
      ```C++
      void print(ostream &os, const BST array[], int numElements) {
        for (int i = 0; i < numElements; ++i) {
          os << array[i]; // Assume `operator<<` is defined for BST object
        }
      }

      BalancedBST bBST[10];
      ...
      print(cout, bBST, 10);
      ```
      Explanation: 
        * `array[i]` stands for `*(array + i)`, where `array` is a pointer to the beginning of the `array`.
        * Distance between `array` and `(array + i)` is `i * sizeof(BST)`.
        * Since `BalancedBST` object contains more data member, and thus is larger than a `BST` object, so pointer arithmetic will be wrong, if we passed in an array of `BalancedBST` objects.
    ### Problem 2: Destructor
      Consider the following function
      ```C++
      void deleteArr(ostream &os, BST array[]) {
        os << "Delete array at address [" << static_cast<void *>(arr) << "]";
        delete [] array;
      }

      BalancedBST bBST = new BalancedBST[10];
      ...
      deleteArr(cout, bBST);
      ```
      Explanation:
        * `delete [] array` is equivalent as calling `array[i].BST::~BST()` for each element in the array, which is an undefined behavior for `BalancedBST` objects.
        * Keep in mind: **The result of deleting an array of derived class objects through a base class pointer is undefined**.

## Item 4: Avoid gratuitous default constructors
  * Default constructors initialize objects without any information from the place where the object is created. This makes perfect sense for pointers (initialized to be `null`), and hash tables, linked lists or maps (initialized to be empty container).
  * For many objects, there is no reasonable way to perform a complete initialization without outside information.
  * Consider the following class, and it lacks a default constructor and may be problematic in three contexts.
    ```C++
    class EquipmentPiece {
    public:
      EquipmentPiece(int ID);
    ...
    };
    ```
    ### Problem 1: **Creation of arrays**
      * Since we have no way to specify arguments for objects in the array, it is not usually possible to create arrays of `EquipmentPiece` objects.
      * There are three ways to get around this restrictions:
        * Approach 1: Provide arguments at the point where the non-heap arrays are defined. Please refer to the following example. But it will be hard to extend this to heap arrays.
          ```C++
          int ID1, ID2, ..., ID10;
          EquipmentPiece bestPieces[] = {
            EquipmentPiece(ID1),
            EquipmentPiece(ID2),
            ...
            EquipmentPiece(ID10)
          };
          ```
        * Approach 2: Use an array of pointers. Please refer to the following examples. Disadvantage would be the need to delete all objects pointed to by the array, and total amount of memory will increase due to the need to more pointers.
          ```C++
          EquipmentPiece * bestPieces[10];
          for (int i = 0; i < 10; ++i) {
            bestPieces[i] = new EquipmentPiece(some ID);
          }
          ```
    ### Problem 2: Ineligible for use with many template-based container classes
    ### Problem 3: Virtual base classes lacking default constructors are a pain to work with