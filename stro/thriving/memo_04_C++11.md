# C++11  (§4)

## C++11: Support for Concurrency (§4.1)

### Threads and Locks (§4.1.2)

On top of the memory model, a threads-and-locks model of concurrency was provided. I consider the threads-and-locks level of concurrency the worst model for application use of concurrency, but it is essential for a language like C++.

- `thread` — a system’s thread of execution, with `join()` and `detach()`
- `mutex` — a system’s mutex with `lock()`, `unlock()`, and RAII ways of getting guaranteed
`unlock()`
- `condition_variable` — a system’s condition variable for communicating events between
threads
- `thread_local` — thread-local storage

The threads-and-locks model requires the use of some form of synchronization to avoid race conditions. C++11 provides standard `mutex`es for that:

```c++
mutex m;
int sh; // shared data

void access() {
    unique_lock<mutex> lck {m};  // aquire mutex
    sh += 7;                     // manipulate shared data
} // release mutex implicitly
```

The `unique_lock` is a RAII object ensuring that the user cannot forget to `unlock()` the `mutex`. These lock objects also provided a way of protecting against the most common form of deadlock:

```c++
void f() {
    // ...
    unique_lock<mutex> lck1 {m1,defer_lock}; // don't yet acquire m1
    unique_lock<mutex> lck2 {m2,defer_lock};
    unique_lock<mutex> lck3 {m3,defer_lock};
    // ...
    lock(lck1,lck2,lck3); // acquire all three mutexes
    // ... manipulate shared data ...
} // implicitly release all mutexes
```

Here, the `lock()` function acquires all `mutex`es *simultaneously* and releases all implicitly. C++17 has an even more elegant solution (§8.4).

### Futures (§4.1.3)

At the 2007 Kona meeting we agreed on a compromise: C++0x (still expected to become C++09) would offer `promise`s and `future`s and a launcher for asynchronous tasks, `async()` [...], "the Kona compromise" pleased nobody and led to some technical problems. However, many users considered it a success — most didn’t know it was a compromise [...].

- `future` — a handle from which you can `get()` a value from a shared one-object buffer, possibly after a wait for the value to be put there by a `promise`.
- `promise` — a handle through which you can `put()` a value to a shared one-object buffer, possibly waking up a `thread` waiting on a `future`.
- `packaged_task` - a class that makes it easy to setup a function to be executed asynchronously on a `thread` with a `future` for its result and a `promise` to return the result.
- `async()` - a function that can launch a task to be executed on another `thread`.

```c++
double comp4(vector<double>& v)
{
    if (v.size()<10000) // is it worth using concurrency?
        return accum(v.begin(),v.end(),0.0); auto v0 = &v[0];
    auto sz = v.size();
    auto f0 = async(accum,v0,v0+sz/4,0.0);
    auto f1 = async(accum,v0+sz/4,v0+sz/2,0.0); 
    auto f2 = async(accum,v0+sz/2,v0+sz*3/4,0.0); 
    auto f3 = async(accum,v0+sz*3/4,v0+sz,0.0);
    return f0.get()+f1.get()+f2.get()+f3.get();
}
```

The `async` wraps the code in a `packaged_task` and manages the setup of a `future` and its `promise` to transmit the result.

A `get()` on the corresponding `future` will now either get a value or throw an exception [...].

## C++11: Simplifying Use (§4.2)

### `auto` and `decltype` (§4.2.1)

`auto` is a static facility allowing the deduction of the (static) type of an object from its initializer.

```c++
auto i = 7;           // i is an int
auto d = 7.2;         // d is a double
auto p = v.begin();   // p gets the type of v's iterator
auto z = y+x;         //z gets the typeof y+x
```

We proposed an operator `decltype` with the reference-preserving semantics:

```c++
template<typename T> void f(T& r) {
    auto v = r;           // v is a T 
    decltype(r) r2 = r;   // r2 is a T& 
    // ...
}
```

A weaker use of `auto` was added in C++11 to move the specification of the return type after the arguments. For example, in C++98, we’d write

```c++
template <typename T>
struct vector98
{
    struct iterator {};
    iterator begin() const;
    iterator end() const;
};

template <typename T>
vector98<T>::iterator vector98<T>::begin() const
{ ... ; }

template <typename T>
vector98<T>::iterator vector98<T>::end() const
{ ... ; }
```

The repetition of vector<T>:: is annoying [...]. C++11 remedied that and improves readability:

```c++
template <typename T>
struct vector11
{
    struct iterator {};
    auto begin() const -> iterator;
    auto end() const -> iterator;
};

template <typename T>
auto vector11<T>::begin() const -> iterator
{ ... ; }

template <typename T>
auto vector11<T>::end() const -> iterator
{ ... ; }
```

C++17 offered `auto` for both arguments and return types for lambdas (§4.3.1), but for functions C++17 offered auto only for return types. C++20 added `auto` for function arguments as part of concepts (§6.4) [...].

### Move Semantics (§4.2.3)

To avoid copying of large amounts of data we then simply had to ensure that that the constructor used to implement the return must copy the handle, rather than all the elements. The C++11 solution to this problem looks like this:

```c++
class Matrix {
    double* elements; // pointer to the elements
    // ...
public:
    Matrix(Matrix&& a) // a move constructor
    {
        elements = a.elements; // copy the handle
        a.elements = nullptr; // now a's destructor has nothing to do
    }
    // ...
};
```

The `&&` indicates that the constructor is a move constructor and `Matrix&&` is called an *rvalue reference*.

The `&&` notation for an rvalue reference, when used for a template parameter, is called a *forwarding reference*:

```c++
template <class T, class A1>
std::shared_ptr<T> factory(A1&& a1)
{
    return std::shared_ptr<T>(new T(std::forward<A1>(a1)));
}
```

The (now) standard-library function `forward` tells the compiler to treat its argument as an rvalue reference, so that a `T`’s move constructor is used to steal that argument (rather than `T`’s copy constructor). It is basically a cast (explicit type conversion) to an rvalue reference.

#### [`forward` Functiom. Cpp Reference](https://en.cppreference.com/w/cpp/utility/forward)

```c++
define in header <utility>

(1)
template< class T >
constexpr T&& forward( std::remove_reference_t<T>& t ) noexcept;

(2)
template< class T >
constexpr T&& forward( std::remove_reference_t<T>&& t ) noexcept;
```

1. Forwards lvalues as either lvalues or as rvalues, depending on `T`
1. Forwards rvalues as rvalues and prohibits forwarding of rvalues as lvalues

When `t` is a forwarding reference (a function argument that is declared as an rvalue reference to a cv-unqualified function template parameter), the overload (1) forwards the argument to another function with the *value category* it had when passed to the calling function.

For example, if used in a wrapper such as the following:

```c++
template<class T>
void wrapper(T&& arg)
{
    // arg is always lvalue
    // Forward as lvalue or as rvalue, depending on T
    foo(std::forward<T>(arg));
}
```

| when `arg` is | `T` is deduced to | `forward` passes `arg` as |
| --- | --- | --- |
| an rvalue `string` | `string` | an rvalue reference |
| a const lvalue `string` | `const string&` | a const lvalue reference |
| a non-const lvalue `string` | `string&` | a non-const lvalue reference |
| | | |

The overload (2) makes it possible to forward a result of an expression (such as function call), which may be rvalue or lvalue, as the original value category of a forwarding reference argument.

#### [Value Categories. Cpp Reference](https://en.cppreference.com/w/cpp/language/value_category)

Each C++ expression (..., etc.) is characterized by two independent properties: a *type* and a *value category*.

- lvalue
  - the name of a variable, a function, a template parameter object (since C++20), or a data member, regardless of type, such as `std::cin` or `std::endl`. Even if the variable's type is rvalue reference, the expression consisting of its name is an lvalue expression;
  - a function call or an overloaded operator expression, whose return type is lvalue reference, such as `std::getline(std::cin, str)`, `std::cout << 1`, `str1 = str2`, or `++it`;
- prvalue (“pure” rvalue)
  - a literal (except for string literal), such as 42, true or nullptr;
  - a function call or an overloaded operator expression, whose return type is non-reference, such as `str.substr(1, 2)`, `str1 + str2`, or `it++`;
- xvalue (an “eXpiring” value)
  - a function call or an overloaded operator expression, whose return type is rvalue reference to object, such as `std::move(x)`;
- glvalue (“generalized” lvalue)
  - A glvalue expression is either lvalue or xvalue.
- rvalue
  - An rvalue expression is either prvalue or xvalue.

### `constexpr` Funtions (§4.2.7)

Our aim was to
- Make compile-time computation type safe
- Generally, improve efficiency by moving computation to compile-time
- Support embedded systems programming (especially ROMs)
- Directly support metaprogramming (as opposed to template metaprogramming (§10.5.2)) 
- Make compile-time programming very similar to "ordinary programming"

The idea was simple:
- allow functions prefixed with constexpr to be used in constant expressions and also
- allow simple user-defined types, called literal types, to be used in constant expressions.

A literal type is basically a type for which all operations are constexpr.

### Attributes (§4.2.10)

Attributes provide a way of associating essentially arbitrary information with an entity in a program.

```c++
[[noreturn]] void forever()
{
    for (;;)
    {
        do_work();
        wait(10s);
    }
}
```

The `[[[noreturn]]` informs a compiler or other tool that `forever()` is never supposed to return so that it can suppress warnings against the lack of a `return`. An attribute is bracketed by `[[ ... ]]`.

To attempt to limit potential damage, we decided that an attribute should imply no change of a program’s semantics. That is, a compiler that ignored an attribute would do no harm. Over the years, this "rule" almost worked. Most standard attributes — though not all — have no semantic effects even though they help with optimizations and error detection.

| Version | Attributes |
| --- | --- |
| `[[noreturn]]`             | indicates that the function does not return |
| `[[carries_dependency]]`   | indicates that dependency chain in release-consume `std::memory_order` propagates in and out of the function |
| `[[deprecated]]`           | |
| `[[deprecated("reason")]]` | indicates that the use of the name or entity declared with this attribute is allowed, but discouraged for some reason |
| `[[fallthrough]]`          | indicates that the fall through from the previous case label is intentional and should not be diagnosed by a compiler that warns on fall-through
| `[[nodiscard]]`            | |
| `[[nodiscard("reason")]]`  | encourages the compiler to issue a warning if the return value is discarded |
| `[[maybe_unused]]`         | suppresses compiler warnings on unused entities, if any |
| `[[likely]]`               | |
| `[[unlikely]]`             | indicates that the compiler should optimize for the case where a path of execution through a statement is more or less likely than any other path of execution |
| `[[no_unique_address]]`    | indicates that a non-static data member need not have an address distinct from all other non-static data members of its class |
| | |

I still see attribute proliferation as a potential danger, but so far the floodgates haven’t opened.
The C++ standard-library uses attributes liberally; `[[nodiscard]]` is particularly popular, especially to protect against potential leaks from unused return values that are resource handles.

## C++11: Improving Support for Generic Programming (§4.3)

### Lambda (§4.3.1)

By default, a lambda cannot refer to names in the local environment, so they are just ordinary functions. However, we can specify that a lambda should "capture" some or all of the variables from its environment. Callbacks are a common use case for lambdas because often the action is unique and need to use some information from the context of the code installing the callback.

```c++
void test()
{
    string s;
    // ... compute a suitable value for s ...
    w.foo_callback([&s](int i){ do_foo(i,s); });
    w.bar_callback([=s](double d){ return do_bar(d,s); }); 
}
```

- The `[&s]` says that `do_foo(i,s)` can use `s` and that `s` is passed (captured) by reference.
- The `[=s]` says that `do_bar(d,s)` can use `s` and that `s` is passed by value. 

If the callback is invoked on the same thread as `test()`, `[&s]` capture is potentially efficient because `s` is not copied. If the callback is invoked on a different thread, `[&s]` capture could be a disaster because `s` could go out of scope before it was used; in that case, we want a copy. 

- A `[=]` capture list means "copy all local variables into the lambda."
- A `[&]` capture list means "the lambda can refer to all local variables by reference" and implies that the lambda can be implemented as simply a local function.

The implementation of a lambda is basically that the compiler builds a suitable function object and passes it. The captured local variables become members initialized by a constructor and the lambda’s code becomes the function object’s application operator. For example, the `bar_callback` becomes:

```c++
struct __XYZ
{
    string s;
    __XYZ(const string& ss ) : s{ss} {}
    int operator()(double d) { return do_bar(d,s); } 
};
```

The return type of a lambda can be deduced from its return statement. If there is no return statement, the lambda doesn’t return anything.

I classify lambdas as support for generic programming because one of the most common uses — and a major motivation — was for the use as arguments to STL algorithms:

```c++
// sort in descending order:
sort(v.begin(),v.end(),[](int x, int y) { return x>y; });

```

### Variadic Templates (§4.3.2)

Variadic templates was a major (sometimes 20 times) improvement over what was an increasingly serious problem as template metaprogramming was beginning to see major use (§10.5.2).

The basic idea is to build up a *parameter pack* recursively and then use it by another recursive pass. The recursive technique was necessary because each element of a parameter pack has its own type (and size).

```c++
template<typename T, typename... Args>
void printf(const char* s, const T& value, const Args&... args)
{
    while (*s) {
        if (*s == '%' && *++s != '%') {
            std::cout << value;
            return printf(++s, args...);
        }
        std::cout << *s++;
    }
    throw std::runtime error("extra arguments provided");
}
```

### Aliases (§4.3.3)

The `using` syntax [...] gave people a choice of notation even where templates are not involved:

```c++
// a pointer to a function taking a vector and returning a double
typedef double (*analysis_fp)(const vector<Student_info>&);
using analysis_fp = double (*)(const vector<Student_info>&);
```

### `tuple`s (§4.3.4)

C++98 had a `pair<T,U>` template; it was mainly used for returning pairs of values, such as two iterators or a pointer and a success indicator.

`Tuple`s are fixed-size heterogeneous containers. They are a general-purpose utility, adding to the expressiveness of the language. Some examples of common uses for `tuple` types are:

- Return types for functions that need to have more than one return type.
- Grouping related types or objects (such as entries in parameter lists) into single entities.
- Simultaneous assignment of multiple values.

```c++
template <typename T>
    requires std::integral<T> || std::floating_point<T>
struct matrix
{
    std::vector<std::vector<T>> val;
};

template <typename T>
auto SVD(matrix<T>& m) -> std::tuple<matrix<T>, std::vector<T>, matrix<T>>
{
    matrix<T> m1, m2;
    vector<T> v;
    // ...
    return {m1, v, m2};  // structured bindings
}

void use()
{
    matrix<int> m1, m2;
    std::vector<int> v;
    auto [m1, v, m2] = SVD(m1);  // structured bindings
}
```

## C++11: Increase Static Type Safety (§4.4)

There are two reasons to rely on static type safety:
- Make it clear what is intended
  - to help programmers directly express ideas
  - to help the compiler catch more errors
- Help the compiler generate better code

## C++11: Support for Library Building (§4.5)

### Implementation Techniques (§4.5.1)

one technique that emerged as key during the development of C++11 and has become almost universal in template-based libraries (including the C++ standard library) known by its odd acronym: SFINAE (Substitution Failure Is Not An Error).

```c++
template<typename T, typename U>
struct pair {
    T first;
    U second;

    enable_if< is_copy_assignable<T>::value && is_copy_assignable<U>::value, pair& >::type
    operator=(const pair&);
};
```

Now, `pair` has a copy assignment if and only if both of its members have one.

The idea is that `enable_if<..., pair&>::type` will become plain `pair&` if the members have copy assignments and fail to instantiate otherwise (because `enable_if` didn’t provide a return type for the assignment). This is where SFINAE comes in: failure to instantiate is not an error; the result of failure is as if the whole declaration wasn’t there.

### Metaprogramming Support (§4.5.2)

Clearly, better support was needed. Attempts to help took two complementary (at least in theory) tracks:

- Language
  - concepts (§6),
  - compile-time functions (§4.2.7), 
  - lambdas (§4.3.1),
  - template aliases (§4.3.3), and
  - more precise specification of template instantiation (§4.5.1).
- Library
  - tuples (§4.3.4),
  - type traits (§4.5.1), and
  - enable_if (§4.5.1).

### noexcept Specifications (§4.5.3)

The original exception design did not have any way to indicate that an exception might be thrown from a function. I still consider that the correct design. To get exceptions accepted for C++98, we had to add exception specifications, a way of listing which exceptions a function could throw. Their use was optional and they were checked at run time. As I feared, that led to maintenance problems, run-time overhead as an exception was repeatedly checked along the unwinding path, and bloated source code. For C++11, exception specifications were deprecated and for C++17, we finally removed them (unanimously).

Without exception specifications, library implementers faced a performance problem: In many important cases, a library implementer needs to know if a copy operation can throw. [...] The performance difference can be very significant and the simplest exception specification `throw()`, throw nothing, was helpful here. So, as exception specifications were pushed into disuse and eventually removed from the standard, we introduce the notion of `noexcept` [...].

```c++
void do_something(int n) noexcept
{
    vector<int> v(n);
    // ...
}
```

There is also a conditional version of noexcept so that people can write templates where the implementation depends on whether a parameter may throw. [...] For example, here is an implementation of a move constructor for `pair` that is defined if and only if both elements of the `pair` have move constructors:

```c++
template<typename First, typename Second>
class pair
{
    template <typename First2, typename Second2>
    pair(pair<First2, Second2>&& rhs)
        noexcept (
            is_nothrow_constructible<First, First2&&>::value
            && is_nothrow_constructible<Second, Second2&&>::value
        )
      : first(move(rhs.first)), second(move(rhs.second))
    {}
    // ...
};
```

