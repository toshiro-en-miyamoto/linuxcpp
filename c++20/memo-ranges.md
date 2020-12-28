# C++20: Ranges

## [CppReference: Ranges Library](https://en.cppreference.com/w/cpp/ranges)

The ranges library provides components for dealing with ranges of elements, including a variety of view adapters.
- Defined in header `<ranges>`
- Defined in namespace `std::ranges`

```c++
namespace std {
    namespace views = ranges::views;
}
```

The namespace alias `std::views` is provided as a shorthand for `std::ranges::views`.

### Range Adaptors

Range adaptors accept `viewable_range` as their first arguments and return a `view`.

If an adaptor takes only one argument, it can also be called using the pipe operator: if `C` is a range adaptor object and `R` is a `viewable_range`, these two expressions are equivalent:

```c++
C(R)
R | C
```

Unary range adaptors can also be chained to produce another range adaptor: if `C` and `D` are range adaptor objects and `R` is a `viewable_range`, then `C | D` is also a range adaptor object, and these two expressions are equivalent:

```c++
R | C | D   // (R | C) | D
R | (C | D)
```

If an adaptor takes multiple arguments, these forms are equivalent:

```c++
adaptor(range, args...)
adaptor(args...)(range)
range | adaptor(args...)
```

In this case, `adaptor(args...)` is a unary range adaptor object.



