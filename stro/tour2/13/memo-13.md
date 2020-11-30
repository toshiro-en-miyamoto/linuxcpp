# Utilities

This section gives a few examples of small, widely useful components. Such components (classes and templates) are often called *vocabulary types* because they are part of the common vocabulary we use to describe our designs and programs.

## Smart Pointers

In `<memory>`, the standard library provides two “smart pointers” to help manage objects on the free store:

- `unique_ptr` to represent unique ownership
- `shared_ptr` to represent shared ownership

```c++
void f()
{
    std::unique_ptr<X> sp { new X };
}
```

`unique_ptr` ensures that its object is properly destroyed whichever way we exit `f()` (by throwing an exception, by executing `return`, or by “falling off the end”).

Its further uses include passing free-store allocated objects in and out of functions:

```c++
std::unique_ptr<X> make_X(int i)
{
    return std::unique_ptr<X> { new X{i} };
}
```

A `unique_ptr` is a handle to an individual object [...] and relies on move semantics to make `return` simple and efficient.

The `shared_ptr` is similar to `unique_ptr` except that `shared_ptrs` are copied rather than moved. The `shared_ptrs` for an object share ownership of an object; that object is destroyed when the last of its `shared_ptrs` is destroyed.

`shared_ptr` provides a form of garbage collection that respects the destructor-based resource management of the memory-managed objects. This is neither cost free nor exorbitantly expensive, but it does make the lifetime of the shared object hard to predict. Use `shared_ptr` only if you actually need shared ownership.

Given `unique_ptr` and `shared_ptr`, we can implement a complete “no naked new” policy (§4.2.2) for many programs. However, these “smart pointers” are still conceptually pointers and therefore only my second choice for resource management – after containers and other types that manage their resources at a higher conceptual level.

We do **not** need to use a pointer to return a collection of objects from a function; a container that is a resource handle will do that simply and efficiently (§5.2.2).

Where do we use “smart pointers” (such as `unique_ptr`) rather than resource handles with operations designed specifically for the resource (such as `vector` or `thread`)? Unsurprisingly, the answer is “when we need pointer semantics.”

- When we share an object, we need pointers (or references) to refer to the shared object, so a `shared_ptr` becomes the obvious choice (unless there is an obvious single owner).
- When we refer to a polymorphic object in classical object-oriented code (§4.5), we need a pointer (or a reference) because we don’t know the exact type of the object referred to (or even its size), so a `unique_ptr` becomes the obvious choice.
- A shared polymorphic object typically requires `shared_ptr`s.

## `move()` and `forward()`

The choice between moving and copying is mostly implicit (§3.6). A compiler will prefer to move when an object is about to be destroyed (as in a `return`) because that’s assumed to be the simpler and more efficient operation.

However, sometimes we must be explicit. For example, a `unique_ptr` is the sole owner of an object. Consequently, it cannot be copied:

```c++
void f1()
{
    auto p = make_unique<X>(2);
    auto q = p;  // compile error
}
```

If you want a `unique_ptr` elsewhere, you must move it.

```c++
void f1()
{
    auto p = make_unique<int>(2);
    auto q = move(p);
    assert(q == nullptr);
}
```

Confusingly, `std::move()` doesn’t move anything. Instead, it casts its argument to an rvalue reference, thereby saying that its argument will not be used again and therefore may be moved (§5.2.2). It should have been called something like `rvalue_cast`.

Like for other casts, there are tempting, but dangerous, uses of `std::move()`. The problem is that a moved-from object is left behind.

```c++
std::string s1 = "Hello";
std::string s2 = "World";
std::vector<std::string> v;
v.push_back(s1);             // push_back() will copy
v.push_back(std::move(s2));  // use a move constructor
std::cout << s1[2];     // write 'l'
std::cout << s2[2];     // crash?
```

I consider this use of `std::move()` to be too error-prone for widespread use. Don’t use it unless you can demonstrate significant and necessary performance improvement. Later maintenance may accidentally lead to unanticipated use of the moved-from object.

The state of a moved-from object is in general unspecified, but all standard-library types leave a moved-from object in a state where it can be destroyed and assigned to. [...] For a container (e.g., `vector` or `string`), the moved-from state will be “empty.” For many types, the default value is a good empty state.

The standard-library `forward()` differs from the simpler `std::move()` by correctly handling subtleties to do with lvalue and rvalue (§5.2.2).

## Range Checking: gsl::span

A key source of range errors is that people pass pointers (raw or smart) and then rely on convention to know the number of elements pointed to. [...] The standard-library `string_view` (§9.3) can help, but that is read-only and for characters only. Most programmers need more.

The Core Guidelines [Stroustrup,2015] offer guidelines and a small Guidelines Support Library [GSL], including a `span` type for referring to a range of elements.

A `span` is basically a `(pointer, length)` pair denoting a sequence of elements.

Like a pointer, a `span` does not own the characters it points to. In that, it resembles a `string_view` (§9.3) and an STL pair of iterators (§12.3).

```c++
#include <cstddef>
#include <span>
#include <algorithm>

template <typename T, std::size_t N, std::size_t M> [[nodiscard]]
constexpr bool contains(std::span<T,N> span, std::span<T,M> sub)
{
    return 
    std::search(span.begin(), span.end(), sub.begin(), sub.end())
        != span.end();
}

int main()
{
    constexpr int seq[] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    static_assert(
        contains(std::span{seq}, std::span{seq, 3})
        && !contains(std::span{seq, 3}, std::span{seq})
    );
}
```

## Specialized Containers

No single container could serve all of these needs because some needs are contradictory, for example, “ability to grow” vs. “guaranteed to be allocated in a fixed location,” and “elements do not move when elements are added” vs. “contiguously allocated.”

### `array`

An `array`, defined in `<array>`, is a fixed-size sequence of elements of a given type where the number of elements is specified at compile time. Thus, an `array` can be allocated with its elements on the stack, in an object, or in static storage.

There is no overhead (time or space) involved in using an `array` compared to using a built-in array (`T[]`). An `array` does not follow the “handle to elements” model of STL containers. Instead, an `array` directly contains its elements.

```c++
#include <array>

int main()
{
    std::array<Circle, 3> ac1;
    // std::size_t sz = 3;
    constexpr std::size_t sz = 3;
    std::array<Circle, sz> ac2;
}
```

The element count is not optional. The element count must be a constant expression. If you need the element count to be a variable, use `vector`.

Why would we use an `array` when `vector` is so much more flexible? An `array` is less flexible so it is simpler. Occasionally, there is a significant performance advantage to be had by directly accessing elements allocated on the stack rather than allocating elements on the free store, accessing them indirectly through the `vector` (a handle), and then deallocating them.

Why would we use an `array` when we could use a built-in array?

```c++
struct Shape { void draw(){} };
struct Circle : Shape {};

int main()
{
    Circle a1[10];
    Shape* p1 = a1;
    p1[3].draw();  // disaster

    std::array<Circle, 10> a2;
    // Shape* p2 = a2;  // compile error
}
```

The “disaster” comment assumes that `sizeof(Shape) < sizeof(Circle)`, so subscripting a `Circle[]` through a `Shape*` gives a wrong offset. All standard containers provide this advantage over built-in arrays.

### `pair` and `tuple`

`std::pair` is a class template that provides a way to store two heterogeneous objects as a single unit. A pair is a specific case of a `std::tuple` with two elements.

- Defined in header `<utility>`
- Defined in namespace `std`

```c++
template< class T1, class T2>
struct pair
{
    using first_type  = T1;
    using second_type = T2;
    T1 first;
    T2 second;
};
```

`equal_range()` returns a range containing all elements equivalent to `value` in the range `[first, last)`.
The first version uses `operator<` to compare the elements, the second version uses the given comparison function `comp`.

```c++
template< class ForwardIt, class T >
constexpr std::pair<ForwardIt,ForwardIt>
    equal_range(ForwardIt first, ForwardIt last,
                const T& value );

template< class ForwardIt, class T, class Compare >
constexpr std::pair<ForwardIt,ForwardIt>
    equal_range(ForwardIt first, ForwardIt last,
                const T& value, Compare comp );
```

We can use that to search in a sorted sequence of `Record`s:

```c++
struct Record
{
    std::string name {};
    int phone {0};
};

constexpr auto less = [](const Record& a, const Record& b)
{ return a.name < b.name; };

int main()
{
    using namespace std::literals;
    std::vector<Record> vr 
    {
        {"a"s, 1111}, {"a"s, 1112},
        {"b"s, 2222},
        {"c"s, 3333}, {"c"s, 3334},
        {"d"s, 4444}
    };

    auto er = equal_range(vr.begin(), vr.end(),
        Record{"c"s}, less);
    for (auto p = er.first; p != er.second; p++)
    { std::cout << p->name << ':' << p->phone << '\n'; }
}
```

Where the names `first` and `second` are too generic, we can use structured binding (§3.6.3):

```c++
    auto [first, last] = equal_range(vr0.begin(), vr0.end(),
        Record{"c"s}, less);
    for (auto p = first; p != last; p++)
    { std::cout << p->name << ':' << p->phone << '\n'; }
```

If you need more than two elements (or less), you can use `tuple` (from `<utility>`).

## Alternatives

The standard library offers three types to express alternatives:

- `variant` to represent one of a specified set of alternatives (in `<variant>`)
- `optional` to represent a value of a specified type or no value (in `<optional>`)
- `any` to represent one of an unbounded set of alternative types (in `<any>`)

### `variant`

A `variant<A,B,C>` is often a safer and more convenient alternative to explicitly using a union (§2.4). Possibly the simplest example is to return either a value or an error code:

```c++
variant<string,int> compose_message(istream& s)
{
    string mess;
    // ... read from s and compose message ...
    if (no_problems)
        return mess;            // return a string
    else
        return error_number;    // return an int
}

int main()
{
    auto m = compose_message(cin));

    if (holds_alternative<string>(m)) {
        cout << m.get<string>();
    }
    else {
        int err = m.get<int>();
        // ... handle error ...
    }
}
```

This style appeals to some people who dislike exceptions (see §3.5.3), but there are more interesting uses. For example, a simple compiler may need to distinguish between different kind of nodes with different representations:

```c++
using Node = variant<Expression,Statement,Declaration,Type>;

void check(Node* p)
{
    if (holds_alternative<Expression>(*p)) {
        Expression& e = get<Expression>(*p);
        // ...
    }
    else if (holds_alternative<Statement>(*p)) {
        Statement& s = get<Statement>(*p);
        // ...
    }
    // ... Declaration and Type ...
}
```

### `optional`

An `optional<A>` can be seen as a special kind of variant (like a `variant<A,nothing>`) or as a generalization of the idea of an `A*` either pointing to an object or being `nullptr`.

```c++
optional<string> compose_message(istream& s)
{
    string mess;
    // ... read from s and compose message ...
    if (no_problems)
        return mess;
    return {};     // the empty optional
}

int main()
{
    if (auto m = compose_message(cin))
        cout << *m;     // note the dereference (*)
    else {
        // ... handle error ...
    }
}
```

The `optional` equivalent to `nullptr` is the empty object, `{}`.

```c++
int cat(optional<int> a, optional<int> b)
{
    int res = 0;
    if (a) res+=*a;
    if (b) res+=*b;
    return res;
}

int x = cat(17,19);
int y = cat(17,{});
int z = cat({},{});
```

If we try to access an `optional` that does not hold a value, the result is undefined; an exception is not thrown. Thus, `optional` is not guaranteed type safe.

### `any`

An `any` can hold an arbitrary type and know which type (if any) it holds. It is basically an unconstrained version of `variant`:

```c++
any compose_message(istream& s)
{
    string mess;
    // ... read from s and compose message ...
    if (no_problems)
          return mess;           // return a string
    else
          return error_number;   // return an int
}

int main()
{
    auto m = compose_message(cin));
    string& s = any_cast<string>(m);
    cout << s;
}
```

If we try to access an `any` holding a different type than the expected one, `bad_any_access` is thrown. There are also ways of accessing an `any` that do not rely on exceptions.

## Allocators

By default, standard-library containers allocate space using `new`. Operators `new` and `delete` provide a general free store (also called dynamic memory or heap) that can hold objects of arbitrary size and user-controlled lifetime. This implies time and space overheads that can be eliminated in many special cases.

The traditional solution to fragmentation problems is to rewrite the code to use a *pool allocator*. A pool allocator is an allocator that manages objects of a single fixed size and allocates space for many objects at a time, rather than using individual allocations. Fortunately, C++17 offers direct support for that. The pool allocator is defined in the `pmr` (“polymorphic memory resource”) sub-namespace of `std`:

```c++
pmr::synchronized_pool_resource pool;

struct Event {
    vector<int> data = vector<int>{512,&pool};
    // let Events use the pool
};

list<shared_ptr<Event>> q {&pool};
// let q use the pool

void producer()
{
    for (int n = 0; n!=LOTS; ++n) {
        scoped_lock lk {m};      // m is a mutex (§15.5)
        q.push_back(
            allocate_shared <
                Event,
                pmr::polymorphic_allocator<Event>
            > {&pool}
        );
        cv.notify_one();
    }
}
```

After eliminating fragmentation, memory use was stable over time so the system could run for months.

## Time

In `<chrono>`, the standard library provides facilities for dealing with time.

```c++
inline auto clock_now()
{
    using namespace std::chrono;
    return high_resolution_clock::now();
}

template<typename T>
inline auto duration_microseconds(T start, T end)
{
    using namespace std::chrono;
    return duration_cast<microseconds>(end-start).count();
}

int main()
{
    int a[1'000'000];
    auto t0 = clock_now();
    for (auto i : a) ;
    auto t1 = clock_now();
    auto d1 = duration_microseconds(t0, t1);
    assert(3'000 < d1 && d1 < 9'000);
}
```

The clock returns a `time_point` (a point in time). Subtracting two `time_points` gives a duration (a period of time). Various clocks give their results in various units of time (the clock I used measures `microseconds`), so it is usually a good idea to convert a duration into a known unit. That’s what `duration_cast` does.

## Function Adaption

The standard-library `function` is a type that can hold any object you can invoke using the call operator `()`. That is, an object of type `function` is a function object (§6.3.2).

```c++
int f1(double);
function<int(double)> fct1 {f1};

int f2(string);
function fct2 {f2}; // fct2's type is function<int(string)>

function fct3 = [](Shape* p) { p−>draw(); };
```

For `fct2`, I let the type of the function be deduced from the initializer: `int(string)`.

Obviously, `function`s are useful for callbacks, for passing operations as arguments, for passing function objects, etc. However, it may introduce some run-time overhead compared to direct calls, and a `function`, being an object, does not participate in overloading.

