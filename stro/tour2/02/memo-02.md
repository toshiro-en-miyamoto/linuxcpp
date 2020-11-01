# User Defined Type
C++’s set of built-in types and operations is rich, but deliberately low-level. They directly and efficiently reflect the capabilities of conventional computer hardware.

Types built out of other types using C++’s abstraction mechanisms are called user-defined types. They are referred to as *classes* and *enumerations*.

## Classes

```C++
class Vector {
public:
     Vector(int s) :elem{new double[s]}, sz{s} { }
     double& operator[](int i) { return elem[i]; }
     int size() { return sz; }
private:
     double* elem;
     int sz;
};
```

Basically, the Vector object is a “handle” containing a pointer to the elements (elem) and the number of elements (sz).
This is the basic technique for handling varying amounts of information in C++: a fixed-size handle referring to a variable amount of data “elsewhere” (e.g., on the free store allocated by new; §4.2.2).

There is no fundamental difference between a struct and a class; a ```struct``` is simply a class with members public by default. For example, you can define constructors and other member functions for a struct.

## Unions
The standard library type, variant, can be used to eliminate most direct uses of unions. A variant stores a value of one of a set of alternative types (§13.5.1).

## Enumerations
```C++
enum class Color { red, blue, green };
enum class Traffic_light { green, yellow, red };

Color col = Color::red;
Traffic_light light = Traffic_light::red;
```
Note that enumerators (e.g., red) are in the scope of their enum class, so that they can be used repeatedly in different enum classes without confusion.

Prefer class enums over “plain” enums to minimize surprises; [CG: Enum.3].

Define operations on enumerations for safe and simple use; [CG: Enum.4].
```C++
Traffic_light& operator++(Traffic_light& t)
{
    switch (t) {
    case Traffic_light::green:
        return t=Traffic_light::yellow;
    case Traffic_light::yellow:
        return t=Traffic_light::red;
    case Traffic_light::red:
        return t=Traffic_light::green;
    }
}

Traffic_light next = ++light;
```
