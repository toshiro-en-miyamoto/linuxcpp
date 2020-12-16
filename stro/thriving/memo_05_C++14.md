# C++14: COMPLETING C++11 (§5)

The C++14 feature set can be summarized as:
• Binary literals — e.g., 0b1001000011110011
• §5.1: Digit separators — for readability, e.g., 0b1001’0000’1111’0011
• §5.2: Variable templates — parameterized constants and variables
• §5.3: Function return type deduction
• §5.4: Generic lambdas
• §5.5: Local variables in constexpr functions
• Move capture — e.g., [p = move(ptr)] { /* ... */ }; move a value into a lambda
• Accessing a tuple by type, e.g., x = get<int>(t);
• User-defined literals in the standard library — e.g., 10i, "Hello!"s, 10s, 3ms, 55us, 17ns

## Digit Separators (§5.1)

```c++
auto a = 1'234'567;   // 1,234,567 (an int)
auto b = 1'234'567s;  // 1,234,567 seconds
```

### Variable Templates (§5.2)

(Variable templates) for specifying constants of various precisions:

```c++
template<typename T>
struct pi
{ static constexpr T value = T(3.1415); };   // class template

template<typename T>
constexpr T pi_func() { return T(3.1415); };   // function template

template<typename T>
constexpr T pi_var = T(3.1415);   // variable template

template<typename T>
constexpr T circular_area(T pi, T r)
{ return pi * r * r; }

int main()
{
    constexpr auto ac1 = circular_area(pi<int>::value, 1);
    static_assert(3 == ac1);
    constexpr auto ac2 = circular_area(pi<double>::value, 1.0);
    static_assert(3.1414 < ac2 & ac2 < 3.1416);

    constexpr auto af1 = circular_area(pi_func<int>(), 1);
    static_assert(3 == af1);
    constexpr auto af2 = circular_area(pi_func<double>(), 1.0);
    static_assert(3.1414 < af2 & af2 < 3.1416);

    constexpr auto av1 = circular_area(pi_var<int>, 1);
    static_assert(3 == av1);
    constexpr auto av2 = circular_area(pi_var<double>, 1.0);
    static_assert(3.1414 < av2 & av2 < 3.1416);
}
```

> (§6.3.6) At the start of the Concepts TS effort, a concept was a `constexpr` function returning a `bool`. That made sense since we saw concepts as compile-time predicates. Then [...] got variable templates accepted into C++14. [...] users of a concept would have to know which style was used in its definition to use parentheses correctly. That quickly became impractical.
> The functional style allows for overloading of concepts, but we had only few examples of concept
overloading and decided we could do without those, so we simplified to use variable templates exclusively for concepts.

### Function Return Type Deduction (§5.3)

C++11 had introduced the ability to deduce the return type of a lambda from its return statement. C++14 extended that to functions

```c++
template<typename T>
auto size(const T& a) { return a.size(); }
```

Users have to be careful, though, because such a function does not provide a stable interface because its type now depends on its implementation and that implementation must now be visible when a use of the function is compiled.

### Generic Lambdas (§5.4)

(Generic lambdas) use auto as a token indicating that a type was to be deduced:

```c++
auto get_size = [](auto& m){ return m.size(); };
```

I think that it was a mistake to introduce generic lambdas in C++14 without also introducing concepts so that the rules and notations for constrained and unconstrained lambda arguments and function arguments were not considered together. The language-technical irregularities stemming from this are (finally) remedied in C++20 (§6.4: C++20 Concepts).

### Local Variables in constexpr Functions (§5.5)

By 2012, people had stopped being scared of `constexpr` functions and started to demand relaxations of the restrictions on their implementations. [...] In particular, local variables and for-loops were allowed.

```c++
constexpr int min(std::initializer_list<int> xs)
{
    int low = std::numeric_limits<int>::max();
    for (int x : xs)
        if (x < low) low = x;
    return low;
}

constexpr int m = min({ 1, 3, 2, 4} );
```

Given a constant expression as argument, this min() can be evaluated at compile time. The local variables (here, `low` and `x`) simply "live" in the compiler. The evaluation cannot have side effects on the environment of a caller.

This relaxation simplified many `constexpr` functions and pleased the many C++ programmers who had not been happy to find that only purely functional expressions of algorithms could be evaluated at compile time. In particular, they wanted loops to avoid recursion. In the longer term, this unleashed demands for further relaxations in C++17 and C++20 (§9.3.3).
