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

## Item 4: Avoid gratuitous default constructors
