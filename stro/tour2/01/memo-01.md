# 1. The Basics
The minimal C++ program is
```C++
int main() {}
```
If no value is returned, the system will receive a value indicating successful completion.

A *declaration* is a statement that introduces an entity into the program. It specifies a type for the entity:
- A *type* defines a set of possible values and a set of operations (for an object).
- An *object* is some memory that holds a value of some type.
- A *value* is a set of bits interpreted according to a type.
- A *variable* is a named object.

## Initialization
C++ offers a variety of notations for expressing initialization, such as the = used above, and a *universal form* based on curly-brace-delimited initializer lists:
```C++
// initialize d1, d2, d3 to 2.3
double d1 = 2.3;
double d2 {2.3};
double d3 = {2.3};   // the = is optional
// a complex number with double-precision floating-point scalars
complex<double> z = 1;
complex<double> z2 {d1,d2};
complex<double> z3 = {d1,d2};
// a vector of ints
vector<int> v {1,2,3,4,5,6};
```

Unfortunately, conversions that lose information, narrowing conversions, such as double to int and int to char, are allowed and implicitly applied when you use = (but not when you use {}). The problems caused by implicit narrowing conversions are a price paid for C compatibility
```C++
int i1 = 7.8;   // i1 becomes 7
int i2 {7.8};   // error
```
## Constants

C++ supports two notions of immutability:

- `const`: meaning roughly “I promise not to change this value.” This is used primarily to specify interfaces so that data can be passed to functions using pointers and references without fear of it being modified. The compiler enforces the promise made by const. The value of a const can be calculated at run time.
- `constexpr`: meaning roughly “to be evaluated at compile time.” This is used primarily to specify constants, to allow placement of data in read-only memory (where it is unlikely to be corrupted), and for performance. The value of a constexpr must be calculated by the compiler.

For example:
```C++
// dmv is a named constant
constexpr int dmv = 17;
// var is not a constant
int var = 17;
// sqv is a named constant, possibly computed at run time
const double sqv = sqrt(var);

// sum will not modify its argument
double sum(const vector<double>&);

// v is not a constant
vector<double> v {1.2, 3.4, 4.5};
// OK: sum(v) is evaluated at run time
const double s1 = sum(v);
// error: sum(v) is not a constant expression
constexpr double s2 = sum(v);
```

For a function to be usable in a constant expression, that is, in an expression that will be evaluated by the compiler, it must be defined constexpr. For example:
```C++
constexpr double square(double x) { return x*x; }
// OK 1.4*square(17) is a constant expression
constexpr double max1 = 1.4*square(17);
// error: var is not a constant expression
constexpr double max2 = 1.4*square(var);
// OK, may be evaluated at run time
const double max3 = 1.4*square(var);
```
A constexpr function can be used for non-constant arguments, but when that is done the result is not a constant expression. We allow a constexpr function to be called with non-constant-expression arguments in contexts that do not require constant expressions. That way, we don’t have to define essentially the same function twice: once for constant expressions and once for variables.

## References
C++ also offers a simpler for-statement, called a range-for-statement, for loops that traverse a sequence in the simplest way:
```C++
int v[] = {0,1,2,3,4,5,6,7,8,9};
constexpr size_t size = sizeof v/sizeof(int);

for (auto i=0; i!=size; i++)
    v[i]++;        // v's elements are incremented

for (auto x : v)   // copy an element in x
    x++;           // v's elements are not changed

for (auto& x : v)  // x refers to an element
    x++;           // v's elements are incremented
```
The for-statement can be read as “set i to zero; while i is not the number of elements in the array v, increment the value of the ith element, and increment i.”
The first range-for-statement can be read as “for every element of v, from the first to the last, place a copy in x, and increment x.”
If we didn’t want to copy the values from v into the variable x, but rather just have x refer to an element, we could write as the second range-for-statement.

Prefer the {}-initializer syntax for declarations with a named type; [CG: ES.23].
```C++
int x {f(99)};
int& r {x};    // bind r to x (r refers to x)
```