#include <iostream>
using namespace std;

class Widget {
public:
    Widget(string name) : d_name(name) {}

    virtual string getName() const {
        return d_name;
    }

private:    
    string d_name;
};

class SpecialWidget: public Widget {
public:
    SpecialWidget(string name, string type = "Default") 
      : Widget(name), d_type(type) { }

    string getType() const {
        return d_type;
    }

private:
    string d_type;
};

void printSpecialWidgetName(SpecialWidget *psw) {
    cout << "Psw name = [" << psw->getName() << "]; type = [" << psw->getType() 
        << "]\n";
}

int main() {
    cout << "Item 2. Sample codes for how CppCast's work.\n";
    SpecialWidget sw("Widget1", "NotDefault");
    const SpecialWidget& csw = sw;
    printSpecialWidgetName(const_cast<SpecialWidget *>(&csw));
        // We here want to remove the constness from csw in order to pass into 
        // `printSpecialWidgetName()` function.
    // printSpecialWidgetName(&csw);  
        // Compilation error: 1st argument ('const SpecialWidget *') would lose const qualifier

    Widget *pw = new SpecialWidget("Widget2", "Special");
    // printSpecialWidgetName(pw);
    printSpecialWidgetName(dynamic_cast<SpecialWidget *>(pw));
        // dynamic_cast<T> cannot be applied to type lacking virtual functions.
    // int a = 5; dynamic_cast<double>(a); => 
    // dynamic_cast<T> cannot be applied to type lacking virtual functions.
    return 0;
}