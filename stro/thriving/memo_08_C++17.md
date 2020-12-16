# C++17: Lost at Sea (§8)

## Constructor Template Argument Deduction (§8.1)

For decades, people have wondered why template arguments could be deduced from function arguments, but not from constructor arguments. [...] in C++17 we can write that last example (p3) without an error, removing the need for the `make_pair()` workaround.

```c++
using namespace std::literals;
constexpr auto hello{"Hello"sv};

// no deduction
constexpr std::pair<std::string_view, int> p1{hello, 1};

// a workaround: deduction by from function arguments
constexpr auto p2 = make_pair(hello, 1);

// C++17 constructor template argument deduction
constexpr std::pair p3 {hello, 1};
```

## Structured Bindings (§8.2)

Structured bindings [...] to simplify notation and eliminate one of the few remaining sources of uninitialized variables.

```c++
template<typename T, typename U>
void print(vector<pair<T,U>>& v)
{
    for (auto [x,y] : v)
        cout << '{' << x << ' ' << y << "}\n";
}
```

C++14 had given us convenient ways of returning multiple values.

```c++
tuple<T1,T2,T3> f(/*...*/)
{
    // ...
    return {a, b, c};
}
```

C++14 did not offer a convenient way of unpacking such multiple return values to match the convenient ways of creating them.

```c++
tuple<T1,T2,T3> res = f();
T1& alpha = get<0>(res);
T2& val = get<1>(res);
T3 err_code = get<2>(res);
```

Many experts preferred to use the standard-library function `tie()` for unpacking `tuple`s: However, to use `tie`, you have to separately declare the variables and name their types to match those of the members of the object returned by `f()` (here, `T1`, `T2`, and `T3`).

```c++
T1 x;
T2 y;
T3 z;
// ...
tie(x,y,z) = f();
```

structured binding was supposed to introduce zero-overhead aliases

```c++
auto [x,y,z] = f();
```

## variant, optional, and any (§8.3)

Alternative types can be represented without run-time overhead by a `union`.

```c++
union U { int i;
char* p; };
U u;
// ...
int x = u.i;
char* p = u.p;
```

This has been used and misused since the earliest days of C as the fundamental way of different types "timesharing" a memory location.

Experienced programmers avoid the problem by encapsulating a union in a class that guarantees proper use.

- `optional<T>` — holds a `T` or nothing
- `variant<T,U>` — holds a `T` or a `U`
- `any` — holds any type

To simplify the use of `variant`s, a visitor mechanism is provided:

### [std::visit](https://en.cppreference.com/w/cpp/utility/variant/visit)

- defined in header `<variant>`
- defined in namespace `std::`

```c++
template <class R, class Visitor, class... Variants>
constexpr R visit( Visitor&& vis, Variants&&... vars );
```

Parameters:
- `vis`: a `Callable` that accepts every possible alternative from every variant
- `vars`: list of variants to pass to the visitor

Return value:
1. The value returned by the selected invocation of the visitor.
1. Nothing if `R` is (possibly cv-qualified) `void`; otherwise the value returned by the selected invocation of the visitor, implicitly converted to `R`.

```c++
#include <variant>
#include <string>
#include <array>
#include <iostream>
#include <type_traits>

using var_t = std::variant<int, long, double, std::string>;

// helper type for the visitor
template<typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide
template<typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int main()
{
    using namespace std::literals;
    std::array<var_t, 4> vars{1, 2L, 3.4, "hello"s};

    for (auto& var : vars) { // void visitor
        std::visit([](auto&& arg){ std::cout << arg << '\n'; }, var);
    }
    for (auto& var : vars) {
        auto v = std::visit([](auto&& arg) -> var_t { return arg + arg; }, var);
        std::visit([](auto&& arg){ std::cout << arg << '\n'; }, v);
    }
    for (auto& var : vars) {
        std::visit([](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>)
                std::cout << "int: " << arg << '\n';
            else if constexpr (std::is_same_v<T, long>)
                std::cout << "long: " << arg << '\n';
            else if constexpr (std::is_same_v<T, double>)
                std::cout << "double: " << arg << '\n';
            else if constexpr (std::is_same_v<T, std::string>)
                std::cout << "string: " << arg << '\n';
        }, var);
    }
    for (auto& var : vars) {
        std::visit(overloaded{
            [](auto arg){ std::cout << arg << '\n'; },
            [](double arg){ std::cout << "double: " << arg << '\n'; },
            [](const std::string& arg) { std::cout << "string: " << arg << '\n'; }
        }, var);
    }
}
```

> That overloaded template really ought to be standard. It is simple only to people comfortable with variadic templates (§4.3.2) and template argument deduction guides (§8.1).

## Concurrency (§8.3)

In C++17, the use of locking became significantly easier by the addition of

- `scoped_lock` — to acquire an arbitrary number of locks without the possibility of deadlock
- `shared_mutex` and `shared_lock` — to implement reader-writer locks

For example, we can acquire multiple locks without fear of deadlock:

```c++
void f() {
    // acquire all three locks
    scoped_lock lck {mutex1,mutex2,mutex3};
}   // implicitly release all mutexes
```

C++11 and C++14 failed to give us reader-writer locks. [...] C++17 remedied that by adding `shared_mutex`:

```c++
shared_mutex mx;

void reader() {
    // willing to share access with other readers
    shared_lock lck {mx};
    // ... read ...
}
void writer() {
    // a writer needs exclusive (unique) access
    unique_lock lck {mx};
    // ... write ...
}
```

Many readers can "share" the lock (i.e., enter the critical section at the same time) whereas the writer requires exclusive access.

