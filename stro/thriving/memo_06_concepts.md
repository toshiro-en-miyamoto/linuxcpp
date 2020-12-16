# Concepts (§6)

```c++
// C++98: no way for specifying interface
template<typename Iter, typename Value>
Iter find(Iter first, Iter last, const Value& val)
{
    while (first!=last && *first==val)
        first++;
    return first;
}
```

(the template) says that
- the first template argument must be a forward iterator.
- the second template argument type must be comparable to the value type of that iterator
using `==`.
- the first two function arguments must denote a sequence.

These requirements are implicit in the code: all the compiler has to go by the use of the template parameters in the function body. The result is great flexibility, splendid generated code for correct calls, and spectacularly bad error messages for incorrect calls. The obvious solution is to specify the first two requirements as part of the template’s interface:

```c++
// C++20: using "concepts"
#include <iterator>

template<std::forward_iterator Iter, typename Value>
    requires std::equality_comparable_with<Value, typename Iter::value_type>
Iter find(Iter first, Iter last, const Value& val)
{
    while (first!=last && *first==val)
        first++;
    return first;
}
```

## The Prehistory of Concepts (§6.1)

The lack of well-specified interfaces led to the spectacularly bad error messages we saw over the years and still in C++17. [...] We (obviously) needed a simple way of specifying the requirements of a template on its template arguments that didn’t imply run-time overheads.

Alex Stepanov used the name "concept" for such requirements in lectures in the late 1990s and documented that in [Dehnert and Stepanov 2000]. I mention this because many have conjectured that concepts were derived from Haskell type classes and misnamed. Alex used the name "concept" because concepts were meant to represent fundamental concepts from an application domain, such as algebra.

We knew of Haskell type classes, but they were not significant influences on the current C++ design because we considered them too inflexible.

### Concepts and Types (§6.3.5)

Where a generic use fits the type-of-type pattern, concepts supports it quite elegantly.

- A *type* specifies the set of operations that can be applied to an object (implicitly and explicitly), relies on function declarations and language rules, and specifies how an object is laid out in memory.
- A *concept* specifies the set of operations that can be applied to an object (implicitly and explicitly), relies on use patterns reflecting function declarations and language rules, and says nothing about the layout of the object. Thus, a concept is a kind of interface.

Except for defining layout, they are very similar. Concepts can even be used to constrain the type of variables with their type determined by their initializer.

```c++
#include <concepts>

template<typename T>
concept integer = std::same_as<T, int> || std::same_as<T, long> || std::same_as<T, long long>;

constexpr integer auto x1 = 7;
constexpr int x2 = 9;

constexpr integer auto y1 = x1 + x2;
constexpr int y2 = x2 + x1;

constexpr integer auto add(integer auto a, integer auto b)
{
    return a + b;
}

int main()
{
    constexpr auto z1 = add(x1, y1);
    static_assert(23 == z1);
}
```

To get that example to work in C++20, we have to add a logically redundant `auto` after each use of the concept `integer`. On the other hand, in C++20, we can use the standard-library concept `integral` instead of the obviously incomplete `integer`.


### Improvement (§6.3.6)

At the start of the Concepts TS effort, a concept was a `constexpr` function returning a `bool`. That made sense since we saw concepts as compile-time predicates.

```c++
template<typename T>
concept bool Sequence = Has_begin<T> && Has_end<T>;
```

We always knew that explicitly mentioning that a concept returned a `bool` was redundant. After all, a concept is by definition a predicate. [...] So we fixed it and no longer have to mention `bool`.

```c++
template<typename T>
concept Sequence = Has_begin<T> && Has_end<T>;
```

## C++20 Concepts (§6.4)

- explicit `requires`-clauses for full generality; e.g., `requires Sortable<S>`
- the shorthand notation for type-of-type uses; e.g., `template<Sortable S>`
- an `auto` to identify template arguments using the natural notation (*), e.g., `void f(Concept
auto&)`

(*) The "natural notation" was renamed the "abbreviated syntax" [...].

```c++
#include <concepts>

template<typename T> requires std::integral<T>
constexpr T f1(T v)
{ return v * v; }

template<std::integral T>
constexpr T f2(T v)
{ return v * v; }

constexpr std::integral auto f3(std::integral auto v)
{ return v * v; }

int main()
{
    constexpr auto x1 = f1(4);
    static_assert(16 == x1);
    static_assert(16 == f2(4));
    static_assert(16 == f3(4));
}
```

## T.20: Avoid "concepts" without meaningful semantics
[C++ Core Guideline T20](https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#t20-avoid-concepts-without-meaningful-semantics)

Concepts are meant to express semantic notions, such as "a number", "a range" of elements, and "totally ordered." Simple constraints, such as "has a `+` operator" and "has a `>` operator" cannot be meaningfully specified in isolation and should be used only as building blocks for meaningful concepts, rather than in user code.

Example, bad (using TS concepts)

```c++
template<typename T>
concept Addable = has_plus<T>;    // bad; insufficient

template<Addable N> auto algo(const N& a, const N& b) // use two numbers
{
    // ...
    return a + b;
}

int x = 7;
int y = 9;
auto z = algo(x, y);   // z = 16

string xx = "7";
string yy = "9";
auto zz = algo(xx, yy);   // zz = "79"
```

Maybe the concatenation was expected. More likely, it was an accident. Defining minus equivalently would give dramatically different sets of accepted types. This `Addable` violates the mathematical rule that addition is supposed to be commutative: `a+b == b+a`.

The ability to specify a meaningful semantics is a defining characteristic of a true concept, as opposed to a syntactic constraint.

Example (using TS concepts)

```c++
template<typename T>
// The operators +, -, *, and / for a number are assumed to follow the usual mathematical rules
concept Number = has_plus<T>
                 && has_minus<T>
                 && has_multiply<T>
                 && has_divide<T>;

template<Number N> auto algo(const N& a, const N& b)
{
    // ...
    return a + b;
}

int x = 7;
int y = 9;
auto z = algo(x, y);   // z = 16

string xx = "7";
string yy = "9";
auto zz = algo(xx, yy);   // error: string is not a Number
```

Concepts with multiple operations have far lower chance of accidentally matching a type than a single-operation concept.



