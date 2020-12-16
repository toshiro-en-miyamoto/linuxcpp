# Error Handling (§7)

As is often the case with C++, the problem is not that we don’t have a solution, but that we have many. It is fundamentally hard to address the diverse needs of the C++ community with a single mechanism, but looking at a subset of the problem people often think they have *the* solution.

## Real-World Problems (§7.2)

The problem of multiple error-reporting schemes is felt most acutely by writers of foundational libraries. They cannot know what their users prefer and their users may very well have many different preferences. The authors of the C++17 filesystem library (§8.6) chose to duplicate their interfaces: for each operation, they offer two functions, one that throws in case of error and another that sets a standard-library error_code passed to it as an argument:

```c++
bool create_directory(const filesystem::path& p); // throws in case of error
bool create_directory(const filesystem::path& p, error_code& ec) noexcept;
```

## Back to Basic (§7.5)

Fundamentally, I think C++ needs two error-handling mechanisms:
- Exceptions — for errors that are rare or cannot be handled by an immediate caller.
- Error codes for errors that can be handled by an immediate caller (often "disguised" in easy-
to-use test operations or returned from a function as a (value,error-code) pair).
