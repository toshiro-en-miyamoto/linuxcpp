# Algorithms

We can simply and efficiently sort a `vector` of `Entry`s and place a copy of each unique `vector` element on a `list`.

```c++
struct Entry
{
    std::string name{};
    int phone{0};
};

bool operator<(const Entry& a, const Entry& b)
{ return a.name < b.name; }

bool operator==(const Entry& a, const Entry& b)
{ return (a.name == b.name) & (a.phone == b.phone); }

void f1(std::vector<Entry>& v, std::list<Entry>& l)
{
    std::sort(v.begin(), v.end());
    unique_copy(v.begin(), v.end(), l.begin());
}
```

For this to work, less than (`<`) and equal (`==`) must be defined for `Entry`s.

```c++
int main()
{
    std::vector<Entry> v1{ Entry{"x", 1234}, Entry{"y", 3456} };

    std::list<Entry> l1(v1.size());
    f1(v1, l1);
    std::vector<Entry> v2{ l1.begin(), l1.end() };
    assert(v1 == v2);
}
```

A standard algorithm is expressed in terms of (half-open) sequences of elements. A *sequence* is represented by a pair of iterators specifying the first element and the one-beyond-the-last element: `[c.begin(), c.end())`.

For writing (output), you need only to specify the first element to be written (`unique_copy(..., ..., l1.begin())`). If more than one element is written, the elements following that initial element will be overwritten. Thus, to avoid errors, `l1` must have at least as many elements as there are unique values in `vec` (`std::list<Entry> l1(v1.size())`).

If we wanted to place the unique elements in a new container, we could have written:

```c++
std::list<Entry> f2(std::vector<Entry>& v)
{
    std::sort(v.begin(), v.end());
    std::list<Entry> l;
    unique_copy(v.begin(), v.end(), back_inserter(l));
    return l;
}

int main()
{
    std::vector<Entry> v1{ Entry{"x", 1234}, Entry{"y", 3456} };
    std::list<Entry> l2{ f2(v1) };
    assert(l1 == l2);
}
```

The call `back_inserter(l)` constructs an iterator for `l` that adds elements at the end of a container, extending the container to make room for them. This saves us from first having to allocate a fixed amount of space and then filling it.

The standard-library `list` has a `move` constructor that makes returning `list` by value efficient.

## Use of Iterators

The standard algorithm `find` looks for a value in a sequence and returns an iterator to the element found. Like many standard-library search algorithms, `find` returns `end()` to indicate “not found.”

```c++
bool has_c2(const std::string& s, char c)
{ return find(s.begin(), s.end(), c) != s.end(); }

int main()
{
    using namespace std::string_literals;
    const auto s{ "tactics"s };
    assert(has_c2(s, 'c'));
}
```

A more interesting exercise would be to find the location of all occurrences of a character in a `string`. We can return the set of occurrences as a `vector` of string iterators. Returning a `vector` is efficient because `vector` provides `move` semantics.

```c++
std::vector<std::string::iterator> find_c1(std::string& s, char c)
{
    std::vector<std::string::iterator> v;
    for (auto p = s.begin(); p != s.end(); p++)
    { if (*p == c) v.push_back(p); }
    return v;
}
```

We iterate through the string using a conventional loop, moving the iterator `p` forward one element at a time using `++` and looking at the elements using the dereference operator `*`.

We could generalize:

```c++
template<typename C, typename E>
std::vector<typename C::iterator> find_c2(C& c, E e)
{
    std::vector<typename C::iterator> v;
    for (auto p = c.begin(); p != c.end(); p++)
    { if (*p == e) v.push_back(p); }
    return v;
}
```

The `typename` is needed to inform the compiler that `C`’s iterator is supposed to be a type and not a value of some type, say, the integer 7. We can hide this implementation detail by introducing a type alias (§6.4.2) for `Iterator`:

```c++
template<typename T>
using Iterator = typename T::iterator;

template<typename C, typename E>
std::vector<Iterator<C>> find_c3(C& c, E e)
{
    std::vector<Iterator<C>> v;
    for (auto p = c.begin(); p != c.end(); p++)
    { if (*p == e) v.push_back(p); }
    return v;
}
```

We can now write:

```c++
int main()
{
    using namespace std::string_literals;
    auto s{ "tactics"s };
    int a{0};
    for (auto p : find_c3(s, 'c'))
    { if (*p == 'c') a++; }
    assert(a == 2);

    std::list<int> li{1, 2, 1, 4};
    a = 0;
    for (auto p : find_c3(li, 1))
    { if (*p == 1) a++; }
    assert(a == 2);

    std::vector<std::string> vs{"am", "pm", "am"};
    a = 0;
    for (auto p : find_c3(vs, "am"))
    { if (*p == "am") a++; }
    assert(a == 2);
}
```

## Iterator Types

Any particular iterator is an object of some type. There are, however, many different iterator types, because an iterator needs to hold the information necessary for doing its job for a particular container type. These iterator types can be as different as the containers and the specialized needs they serve.

What is common for all iterators is their semantics and the naming of their operations. For example, applying `++` to any iterator yields an iterator that refers to the next element. Similarly, `*` yields the element to which the iterator refers.

Furthermore, users rarely need to know the type of a specific iterator; each container “knows” its iterator types and makes them available under the conventional names `iterator` and `const_iterator`.

```c++
template<typename T>
using Const_iterator = typename T::const_iterator;

template<typename C, typename E>
std::vector<Const_iterator<C>> find_c4(const C& c, E e)
{
    std::vector<Const_iterator<C>> v;
    for (auto p = c.cbegin(); p != c.cend(); p++)
    { if (*p == e) v.push_back(p); }
    return v;
}
```

## Algorithms Overview

A general definition of an algorithm is “a finite set of rules which gives a sequence of operations for solving a specific set of problems ..." In the context of the C++ standard library, an algorithm is a function template operating on sequences of elements.

The algorithms are defined in namespace `std` and presented in the `<algorithm>` header. These standard-library algorithms all take sequences as inputs. A half-open sequence from `b` to `e` is referred to as `[b:e)`.

These algorithms, and many more (e.g., §14.3), can be applied to elements of containers, `string`s, and built-in arrays.

| Argorithm | Description |
| --- | --- |
| f=for_each(b,e,f)          | For each element `x` in `[b:e)` do `f(x)` |
| p=find(b,e,x)              | `p` is the first `p` in `[b:e)` so that `*p==x` |
| p=find_if(b,e,f)           | `p` is the first `p` in [`b:e)` so that `f(*p)` |
| n=count(b,e,x)             | `n` is the number of elements `*q` in `[b:e)` so that `*q==x` |
| n=count_if(b,e,f)          | `n` is the number of elements `*q` in `[b:e)` so that `f(*q)` |
| replace(b,e,v,v2)          | Replace elements `*q` in `[b:e)` so that `*q==v` with `v2` |
| replace_if(b,e,f,v2)       | Replace elements `*q` in `[b:e)` so that `f(*q)` with `v2` |
| p=copy(b,e,out)            | Copy `[b:e)` to `[out:p)` |
| p=copy_if(b,e,out,f)       | Copy elements `*q` from `[b:e)` so that `f(*q)` to `[out:p)` |
| p=move(b,e,out)            | Move `[b:e)` to `[out:p)` |
| p=unique_copy(b,e,out)     | Copy `[b:e)` to `[out:p)`; don’t copy adjacent duplicates |
| sort(b,e)                  | Sort elements of `[b:e)` using `<` as the sorting criterion |
| sort(b,e,f)                | Sort elements of `[b:e)` using `f` as the sorting criterion |
| (p1,p2)=equal_range(b,e,v) | `[p1:p2)` is the subsequence of the sorted sequence `[b:e)` with the value `v`; basically a binary search for `v` |
| p=merge(b,e,b2,e2,out)     | Merge two sorted sequences `[b:e)` and `[b2:e2)` into `[out:p)` |
| p=merge(b,e,b2,e2,out,f)   | Merge two sorted sequences `[b:e)` and `[b2:e2)` into `[out:p)` using `f` as the comparison |

Some algorithms, such as `replace`() and `sort()`, modify element values, but no algorithm adds or subtracts elements of a container. The reason is that a sequence does not identify the container that holds the elements of the sequence. To add or delete elements, you need something that knows about the container (e.g., a `back_inserter`; §12.1) or directly refers to the container itself (e.g., `push_back()` or `erase()`; §11.2).

```c++
int main()
{
    std::vector<int> v1{1, 2, 1, 1, 4, 1};
    std::vector<int> v2;
    unique_copy(v1.begin(), v1.end(), back_inserter(v2));
    std::vector<int> v3{1, 2, 1, 4, 1};
    assert(v2 == v3);
}
```

## Concepts

`range` is a generalization of the C++98 sequences defined by `begin()`/`end()` pairs. `range` is a concept specifying what it takes to be a sequence of elements. It can be defined by

- A `{begin,end}` pair of iterators
- A `{begin,n}` pair, where `begin` is an iterator and `n` is the number of elements
- A `{begin,pred}` pair, where `begin` is an iterator and `pred` is a predicate; if `pred(p)` is true for the iterator `p`, we have reached the end of the sequence. This allows us to have infinite sequences and sequences that are generated “on the fly.”

In general, where a standard-library algorithm requires a sequence defined by a pair of iterators, C++20 will allow a `range` as a notationally simpler alternative.

## Concepts library 
[Concepts library](https://en.cppreference.com/w/cpp/concepts)

The concepts library provides definitions of fundamental library concepts that can be used to 
- perform compile-time validation of template arguments and 
- perform function dispatch based on properties of types. 

These concepts provide a foundation for equational reasoning in programs.

Most concepts in the standard library impose both *syntactic* and *semantic* requirements. It is said that 
- a standard concept is *satisfied* if its syntactic requirements are met, and 
- a standard concept is *modeled* if it is satisfied and its semantic requirements (if any) are also met.

In general, only the syntactic requirements can be checked by the compiler. If the validity or meaning of a program depends whether a sequenced of template arguments models a concept, and the concept is satisfied but not modeled, or if a semantic requirement is not met at the point of use, the program is ill-formed, no diagnostic required.

- Defined in header `<concepts>`
- Defined in namespace `std`

### Core language concepts

| Concept | Specifies that |
| --- | --- |
| same_as               | a type is the same as another type |
| derived_from          | a type is derived from another type |
| convertible_to        | a type is implicitly convertible to another type |
| common_reference_with | two types share a common reference type |
| common_with           | two types share a common type |
| integral              | a type is an integral type |
| signed_integral       | a type is an integral type that is signed |
| unsigned_integral     | a type is an integral type that is unsigned |
| floating_point        | a type is a floating-point type |
| assignable_from       | a type is assignable from another type |
| swappable             | a type can be swapped |
| swappable_with        | two types can be swapped with each other |
| destructible          | an object of the type can be destroyed |
| constructible_from    | a variable of the type can be constructed from or bound to a set of argument types |
| default_initializable | an object of a type can be default constructed |
| move_constructible    | an object of a type can be move constructed |
| copy_constructible    | an object of a type can be copy constructed and move constructed |
| | |

### Comparison concepts

| Concept | Specifies that |
| --- | --- |
| boolean-testable         | a type can be used in Boolean contexts |
| equality_comparable      | operator `==` is an equivalence relation |
| equality_comparable_with | operator `==` is an equivalence relation |
| totally_ordered          | the comparison operators on the type yield a total order |
| totally_ordered_with     | the comparison operators on the type yield a total order |
| | |

### Object concepts

| Concept | Specifies that |
| --- | --- |
| movable     | an object of a type can be moved and swapped |
| copyable    | an object of a type can be copied, moved, and swapped |
| semiregular | an object of a type can be copied, moved, swapped, and default constructed |
| regular     | a type is both `semiregular` and `equality_comparable` |
| | |

### Callable concepts

| Concept | Specifies that |
| --- | --- |
| invocable            | a callable type can be invoked with a given set of argument types |
| regular_invocable    | a callable type can be invoked with a given set of argument types |
| predicate            | a callable type is a Boolean predicate |
| relation             | a callable type is a binary relation |
| equivalence_relation | a relation imposes an equivalence relation |
| strict_weak_order    | a relation imposes a strict weak ordering |
| | |

The concept `predicate<F, Args...>` specifies that F is a predicate:

```c++
template < class F, class... Args >
concept predicate =
    std::regular_invocable<F, Args...> &&
    boolean-testable<std::invoke_result_t<F, Args...>>;
```

The concept `relation<R, T, U>` specifies that `R` defines a binary relation over the set of expressions whose type and value category are those encoded by either `T` or `U`.

```c++
template <class R, class T, class U>
concept relation =
    std::predicate<R, T, T> && std::predicate<R, U, U> &&
    std::predicate<R, T, U> && std::predicate<R, U, T>;
```

The concept `equivalence_relation<R, T, U>` specifies that the relation `R` imposes an equivalence relation on its arguments. Note: The distinction between `relation` and `equivalence_relation` is purely semantic.

```c++
template < class R, class T, class U >
concept equivalence_relation = std::relation<R, T, U>;
```

The concept `strict_weak_order<R, T, U>` specifies that the relation `R` imposes a strict weak ordering on its arguments. Notes: 
The distinction between `relation` and `strict_weak_order` is purely semantic.

```c++
template < class R, class T, class U >
concept strict_weak_order = std::relation<R, T, U>;
```

## Iterator Concepts
[Iterator library](https://en.cppreference.com/w/cpp/iterator)

C++20 introduces a new system of iterators based on concepts that are different from C++17 iterators. While the basic taxonomy remains similar, the requirements for individual iterator categories are somewhat different.

- Defined in header `<iterator>`
- Defined in namespace `std`

| Concept | Specifies that |
| --- | --- |
| indirectly_readable      | a type is indirectly readable by applying operator `*` |
| indirectly_writable      | a value can be written to an iterator's referenced object |
| weakly_incrementable     | a `semiregular` type can be incremented with pre- and post-increment operators |
| incrementable            | the increment operation on a `weakly_incrementable` type is equality-preserving and that the type is `equality_comparable` |
| input_or_output_iterator | objects of a type can be incremented and dereferenced |
| sentinel_for             | a type is a sentinel for an `input_or_output_iterator type` |
| sized_sentinel_for       | the `-` operator can be applied to an iterator and a sentinel to calculate their difference in constant time |
| input_iterator           | a type is an input iterator, that is, its referenced values can be read and it can be both pre- and post-incremented |
| output_iterator          | a type is an output iterator for a given value type, that is, values of that type can be written to it and it can be both pre- and post-incremented |
| forward_iterator         | an input_iterator is a forward iterator, supporting equality comparison and multi-pass |
| bidirectional_iterator   | a `forward_iterator` is a bidirectional iterator, supporting movement backwards |
| random_access_iterator   | a `bidirectional_iterator` is a random-access iterator, supporting advancement in constant time and subscripting |
| contiguous_iterator      | a `random_access_iterator` is a contiguous iterator, referring to elements that are contiguous in memory |
| | |

> Equality preservation ("A Tour of C++, 2nd Edition" by Bjarne Stroustrup)
>
> A function `f()` is equality preserving if `x==y` implies that `f(x)==f(y)`.

> Sentinel ("A Tour of C++, 2nd Edition" by Bjarne Stroustrup)
>
> The basic idea of a sentinel is that we can iterate over a range starting at an iterator until the predicate becomes true for an element. That way, an iterator `p` and a sentinel `s` define a range `[p:s(*p))`.

## Algorithm Concepts
[Iterator library](https://en.cppreference.com/w/cpp/iterator)

C++20 also provides a set of concepts and related utility templates designed to ease constraining common algorithm operations.

- Defined in header `<iterator>`
- Defined in namespace `std`

### Indirect callable concepts

| Concept | Specifies that |
| --- | --- |
| indirectly_unary_invocable         | a callable type can be invoked with the result of dereferencing an `indirectly_readable type`        |
| indirectly_regular_unary_invocable | a callable type can be invoked with the result of dereferencing an `indirectly_readable` type |
| indirect_unary_predicate           | a callable type, when invoked with the result of dereferencing an `indirectly_readable` type, satisfies predicate |
| indirect_binary_predicate          | a callable type, when invoked with the result of dereferencing two `indirectly_readable` types, satisfies predicate |
| indirect_equivalence_relation      | a callable type, when invoked with the result of dereferencing two `indirectly_readable` types, satisfies `equivalence_relation` |
| indirect_strict_weak_order         | a callable type, when invoked with the result of dereferencing two `indirectly_readable` types, satisfies `strict_weak_order` |
| | |

### Common algorithm requirements

| Concept | Specifies that |
| --- | --- |
| indirectly_movable           | values may be moved from an `indirectly_readable` type to an `indirectly_writable` type |
| indirectly_movable_storable  | values may be moved from an `indirectly_readable` type to an `indirectly_writable` type and that the move may be performed via an intermediate object |
| indirectly_copyable          | values may be copied from an `indirectly_readable` type to an `indirectly_writable` type |
| indirectly_copyable_storable | values may be copied from an `indirectly_readable` type to an `indirectly_writable` type and that the copy may be performed via an intermediate object |
| indirectly_swappable         | the values referenced by two `indirectly_readable` types can be swapped |
| indirectly_comparable        | the values referenced by two `indirectly_readable` types can be compared |
| permutable                   | the common requirements of algorithms that reorder elements in place |
| mergeable                    | the requirements of algorithms that merge sorted sequences into an output sequence by copying elements |
| sortable                     | the common requirements of algorithms that permute sequences into ordered sequences |
| | |

## Ranges Concepts
[Ranges library](https://en.cppreference.com/w/cpp/ranges)

- defined in header `<ranges>`
- defined in namespace `std::ranges`

| Concept | Specifies that |
| --- | --- |
| range               | specifies that a type is a range, that is, it provides a begin iterator and an end sentinel |
| borrowed_range      | specifies that a type is a range and iterators obtained from an expression of it can be safely returned without danger of dangling |
| sized_range         | specifies that a range knows its size in constant time |
| view                | specifies that a range is a view, that is, it has constant time copy/move/assignment |
| input_range         | specifies a range whose iterator type satisfies `input_iterator` |
| output_range        | specifies a range whose iterator type satisfies `output_iterator` |
| forward_range       | specifies a range whose iterator type satisfies `forward_iterator` |
| bidirectional_range | specifies a range whose iterator type satisfies `bidirectional_iterator` |
| random_access_range | specifies a range whose iterator type satisfies `random_access_iterator` |
| contiguous_range    | specifies a range whose iterator type satisfies `contiguous_iterator` |
| common_range        | specifies that a range has identical iterator and sentinel types |
| viewable_range      | specifies the requirements for a range to be safely convertible to a view |
| | |
