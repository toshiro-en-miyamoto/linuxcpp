# C++20: A Struggle for Direction (§9)

## Design Principles (§9.1)

Today, some of the most powerful design techniques combine
- aspects of traditional object-oriented programming,
- aspects of generic programming,
- aspects of functional programming, and
- some traditional imperative techniques.

Such combinations, rather than theoretical purity, are the ideal.

C++ relies critically on static type safety for expressiveness, performance, and safety. The ideal is: Complete type-safety and resource-safety (no memory corruption and no leaks). This is achievable without added overhead, in particular without adding a garbage collector, and without restricting expressibility.

## C++20 Features (§9.3)

The C++20 features are:

- §6.4: Concepts — specification of requirements for generic code
- §9.3.1: Modules — support for modularity for code hygiene and improved compile times
- §9.3.2: Coroutines — stackless coroutines
- §9.3.3: Compile-time computation support
- §9.3.4: <=> — a three-way comparison operator
- §9.3.5: Ranges — a library flexible range abstractions
- §9.3.6: Date — a library providing date types, calendar, and time zones
- §9.3.8: Span — a library providing efficient and safe access to arrays
- §9.3.7: Format — a library providing type-safe printf-like output
- §9.4: Concurrency improvements — such as scoped threads and stop tokens
- §9.5: Many minor features — such as C99-style designated initializers and string literals as
template arguments

The following are not ready for C++20 but may become major features of C++23:

- §8.8.1: Networking — a networking library (sockets, etc.)
- §9.6.2: Static reflection — facilities for generating code based on the surrounding program
- Pattern Matching — selecting code to be executed based on types and object values

Unfortunately, C++20 doesn’t have standard library support for modules and coroutines. That could become a significant problem, but there wasn’t time to get that ready in time to ship on time. C++23 should provide that.

### Coroutines (§9.3.2)

Coroutines offer a model of cooperative multi-tasking that can be far more efficient than use of threads or processes.

### Ranges (§9.3.5)

The range library provides the three major alternatives (now called ranges):
- `(first, one_beyond_last)` for when we know where the beginning and the end of thes equence are (e.g., "sort from the beginning to the end of a `vector`").
- `(first, number_of_elements)` for when we don’t actually need to have the end of a sequence computed (e.g., "look at the first 10 elements of a list").
- `(first, termination_criteria)` for when we use a predicate (e.g., a sentinel) to define the end of the sequence (e.g., "read until end of input").

A `range` is a `concept` [...] and it is what enables the generalization to use ranges, rather than just iterators. That generalization allows for pipelining of algorithms:

```c++
#include <vector>
#include <ranges>
#include <cassert>

int main()
{
    std::vector<int> series{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    int result = 0;
    for (int i : series
        | std::views::filter([](int i){ return i%2 == 0; })
        | std::views::transform([](int i){ return i*2; })
        | std::views::take(5)
    ){
        result += i;
    };
    assert(result == 60);
}
```

Like in Unix, the pipeline operator, |, delivers the output of its left-hand operand as the input to its right-hand operand (e.g., A|B means B(A)). This will get much more interesting once people start using coroutines (§9.3.2) to write pipeline filters.

> The namespace alias `std::views` is provided as a shorthand for `std::ranges::views`. Refer to [cppreference: Range Library](https://en.cppreference.com/w/cpp/ranges).

### Span (§9.3.8)

Span. Out-of-range access, sometimes referred to as buffer overflow, has been a serious
problem from the early days of C.

```c++
void f(int* p, int n) // what is n?
{
    for (int i=0; i<n; ++i)
        p[i] = 7; // OK?
}

int x = 100;
int a[100];
f(a,x); // OK
f(a,x/2); // OK: first half of a 
f(a,x+1); // disaster!
```

How would a tool, e.g., a compiler, know that n was meant to be the number of elements in the array pointed to? How can a programmer get it consistently right in a large program?

Compilers do not catch range-errors and run-time checking of all subscripting has generally been deemed too expensive for production code.

Using `span`, that example can now be written:

```c++
void f(span<int> a) // span holds a pointer and a size
{
    for (int& x : a)
        x=7; //OK
}

int x = 100;
int a[100];
f(a); // template argument deduction: f(span<int>{a,100})
```

The range-`for` extracts the range from the span and traverses exactly the right number of elements (without costly range checking). This is an example of how an appropriate abstraction can simultaneously simplify notation and improve performance. It is easier and cheaper for an algorithm to explicitly use a range (here, `span`) than to check each individual element access.

## Concurrency (§9.4)

Despite valiant efforts and an emerging broad consensus, the hoped-for general concurrency model ("executors") wasn’t ready for C++20 [...].
However, several less dramatic useful improvements were completed in time, including
- `jthread` and stop tokens
- `atomic<shared_ptr<T>>`
- classic semaphores
- barriers and latches

The `jthread` (short for "joining thread") is a thread that obeys RAII; that is, it’s destructor `join`s, rather than terminates, if the `jthread` goes out of scope:

```c++
void some_func()
{
    thread t1;
    jthread t2;
}
```
At the end of scope, `t1`’s destructor terminates the program unless `t1`’s task has completed, `join`ed, or `detach`ed, whereas `t2`’s destructor will wait for its task to complete.

"Stop Tokens" is a solution to the old problem of how to stop a thread when we are no longer interested in its result. The basic idea is to make thread cancellation (§4.1.2) cooperative. If I want a `jthread` to terminate, I set its stop token. It is the thread’s obligation to occasionally test whether the stop token has been set and if so, clean up and exit. This technique is as old as the mountains and works nicely and efficiently for just about every thread that has a main loop where the test of the stop token can be placed.


