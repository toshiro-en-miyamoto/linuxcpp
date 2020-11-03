# Classes
## Concrete Classes
Functions defined in a class are inlined by default. It is possible to explicitly request inlining by preceding a function declaration with the keyword inline.

```C++
class complex {
    double re, im;
public:
    double real() const { return re; }
    void real(double r) { re = r; }
    complex& operator+=(const complex z) {
        re+=z.re;
        im+=z.im;
        return *this;
    }
};
```

The ```const``` specifiers on the functions returning the real and imaginary parts indicate that these functions do not modify the object for which they are called.

Many useful operations do not require direct access to the representation of complex, so they can be defined separately from the class definition:
```C++
complex operator+(complex a, complex b) { return a+=b; }
```

Here, I use the fact that an argument passed by value is copied so that I can modify an argument without affecting the caller’s copy and use the result as the return value.

## Initializing Containers
The ```std::initializer_list``` used to define the initializer-list constructor is a standard-library type known to the compiler: when we use a {}-list, such as {1,2,3,4}, the compiler will create an object of type initializer_list to give to the program.

```c++
#include <algorithm>
#include <iostream>

class Vector {
    double *elem;
    size_t sz;
public:
    Vector(std::initializer_list<double> list)
      : elem { new double[list.size()] },
        sz { list.size() }
    {
        std::copy(list.begin(), list.end(), elem);
    }

    double& operator[](int i) const { return elem[i]; }
    size_t size() const { return sz; }
};

int main() {
    Vector v {1, 2, 3, 4, 5};
    for (size_t i = 0; i < v.size(); i++)
        std::cout << v[i] << " ";
    std::cout << std::endl;
}
```

Unfortunately, the standard-library uses unsigned integers (```size_t, aka 'unsigned long'```) for sizes and subscripts.

## Abstract Types
Types such as complex and Vector are called *concrete types* because their representation is part of their definition. In that, they resemble built-in types.

In contrast, an *abstract type* is a type that completely insulates a user from implementation details. To do that, we decouple the interface from the representation and give up genuine local variables. Since we don’t know anything about the representation of an abstract type (not even its size), we must allocate objects on the free store (§4.2.2) and access them through references or pointers (§1.7, §13.2.1).

```c++
class Container {
public:
    virtual double& operator[](int) = 0;
    virtual int size() const = 0;
    virtual ~Container() {}
};

double accum(Container& c) {
    const int sz = c.size();
    double a = 0;
    for (int i = 0; i < sz; i++)
        a += c[i];
    return a;
}
```

The word ```virtual``` means “may be redefined later in a class derived from this one.”
Unsurprisingly, a function declared virtual is called a *virtual function*. A class derived from Container provides an implementation for the Container interface. The curious ```=0``` syntax says the function is *pure virtual*; that is, some class derived from Container must define the function.

As is common for abstract classes, Container does not have a constructor. On the other hand, Container does have a destructor and that destructor is virtual, so that classes derived from Container can provide implementations.

```c++
class Vector_container : public Container {
     Vector v;
public:
     Vector_container(int s) : v(s) { }
     ~Vector_container() {}

     double& operator[](int i) override { return v[i]; }
     int size() const override { return v.size(); }
};
```

The members ```operator[]()``` and ```size()``` are said to override the corresponding members in the base class Container. I used the explicit ```override``` to make clear what’s intended. The use of override is optional, but being explicit allows the compiler to catch mistakes, such as misspellings of function names or slight differences between the type of a virtual function and its intended overrider.

The destructor (```~Vector_container()```) overrides the base class destructor (```~Container()```). Note that the member destructor (```~Vector()```) is implicitly invoked by its class’s destructor (~Vector_container()).

## Class Hierarchy

A virtual destructor is essential for an abstract class because an object of a derived class is usually manipulated through the interface provided by its abstract base class. In particular, it may be deleted through a pointer to a base class. Then, the virtual function call mechanism ensures that the proper destructor is called. That destructor then implicitly invokes the destructors of its bases and members.

Concrete classes – especially classes with small representations – are much like built-in types: we define them as local variables, access them using their names, copy them around, etc.

Classes in class hierarchies are different: we tend to allocate them on the free store using new, and we access them through pointers or references.

### Dynamic Cast Conversion
[Cpp Reference, dynamic_cast](https://en.cppreference.com/w/cpp/language/dynamic_cast)

Safely converts pointers and references to classes up, down, and sideways along the inheritance hierarchy.

```c++
dynamic_cast < new-type > ( expression )
```
<dl>
<dt>new-type</dt>
<dd>pointer to complete class type, reference to complete class type, or pointer to (optionally cv-qualified) void</dd>
<dt>expression</dt>
<dd>lvalue (until C++11) glvalue (since C++11) of a complete class type if new-type is a reference, prvalue of a pointer to complete class type if new-type is a pointer</dd>
</dl>

If the cast is successful, dynamic_cast returns a value of type new-type. If the cast fails and new-type is a pointer type, it returns a null pointer of that type. If the cast fails and new-type is a reference type, it throws an exception that matches a handler of type ```std::bad_cast```.

```c++
struct Base {
    // must be polymorphic to use runtime-checked dynamic_cast
    virtual ~Base() {}
};
struct Derived: Base {};

int main() {
    Base* b = new Derived;
    Derived* d = dynamic_cast<Derived*>(b);
    assert(d != nullptr);
}
```

[Cpp Reference, Polymorphic objects](https://en.cppreference.com/w/cpp/language/object#Polymorphic_objects)

Objects of a class type that declares or inherits at least one virtual function are polymorphic objects.
