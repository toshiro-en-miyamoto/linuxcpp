# Retrospective (§11)

## Technical Successes (§11.2)

Building on the previous decades, the key technical advances of the 2000s include:
- The memory model (§4.1.1: C++11)
- Type-safe concurrency support: threads and locks (§4.1.2: C++11), parallel algorithms (§8.5: C++17), joining threads (§9.4: C++20)
- Type deduction: `auto` (§4.2.1: C++11), concepts (§6: C++20), template argument deduction (§8.1: C++17), variadic templates (§4.3.2: C++11)
- Simplification of use: `auto` (§4.2.1: C++11), range-`for`(§4.2.2: C++11), parallel algorithms (§8.5: C++17), ranges (§9.3.5: C++20), lambdas (§4.3.1: C++11)
- Move semantics (§4.2.3: C++11)
- Compile-time programming: `constexpr` (§4.2.7: C++11), compile-time loops (§5.5: C++14), guaranteed compile-time evaluation and containers (§9.3.3: C++20), metaprogramming (§10.5.2)
- Generic programming: the STL (§10.5.1), concepts (§6: C++20), user-defined types as template parameters (§9.3.3: C++20), lambdas (§4.3.1: C++11)
- Metaprogramming (§10.5.2)

## The Future (§11.5)

"work towards having the following things in C++23:"

- Library support for coroutines (§9.3.2)
- A modular standard library (§9.3.1)
- The general asynchronous computing model (executors) (§8.8.1)
- Networking (§8.8.1)

Note the focus is on libraries. "Also make progress on:"

- Static reflection facilities (§9.6.2)
- Functional-programming style pattern matching (§8.2)
- Contracts (§9.6.1)
