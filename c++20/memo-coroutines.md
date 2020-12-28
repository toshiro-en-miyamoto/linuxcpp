# C++20: Coroutines

## [CppReference: Coroutine](https://en.cppreference.com/w/cpp/language/coroutines)

A coroutine is a function that can suspend execution to be resumed later. Coroutines are stackless: they suspend execution by returning to the caller and the data that is required to resume execution is stored separately from the stack. This allows for sequential code that executes asynchronously (e.g. to handle non-blocking I/O without explicit callbacks), and also supports algorithms on lazy-computed infinite sequences and other uses.

A function is a coroutine if its definition does any of the following:

- uses the `co_await` operator to suspend execution until resumed
```c++
task<> tcp_echo_server() {
    char data[1024];
    for (;;) {
        size_t n = co_await socket.async_read_some(buffer(data));
        co_await async_write(socket, buffer(data, n));
    }
}
```
- uses the keyword `co_yield` to suspend execution returning a value
```c++
generator<int> iota(int n = 0) {
    while(true)
        co_yield n++;
}
```
- uses the keyword `co_return` to complete execution returning a value
```c++
lazy<int> f() {
    co_return 7;
}
```

### Restrictions
Coroutines cannot use variadic arguments, plain return statements, or placeholder return types (`auto` or Concept).

`Constexpr` functions, constructors, destructors, and the `main` function cannot be coroutines.

### Execution

Each coroutine is associated with

- the **promise object**, manipulated from inside the coroutine. The coroutine submits its result or exception through this object.
- the **coroutine handle**, manipulated from outside the coroutine. This is a non-owning handle used to resume execution of the coroutine or to destroy the coroutine frame.
- the **coroutine state**, which is an internal, heap-allocated (unless the allocation is optimized out), object that contains
  - the promise object
  - the parameters (all copied by value)
  - some representation of the current suspension point, so that `resume` knows where to continue and `destroy` knows what local variables were in scope
  - local variables and temporaries whose lifetime spans the current suspension point

When a coroutine begins execution, it performs the following:

- allocates the **coroutine state** object using operator `new` (see below)
- copies all **function parameters** to the coroutine state: by-value parameters are moved or copied, by-reference parameters remain references (and so may become dangling if the coroutine is resumed after the lifetime of referred object ends)
- calls the constructor for the **promise object**. If the `promise type` has a constructor that takes all coroutine parameters, that constructor is called, with post-copy coroutine arguments. Otherwise the default constructor is called.
- calls `promise.get_return_object()` and keeps the result in a local variable. The result of that call will be returned to the caller when the coroutine first suspends. Any exceptions thrown up to and including this step propagate back to the caller, not placed in the promise.
- calls `promise.initial_suspend()` and `co_await`'s its result. Typical `promise type`s either return a `suspend_always`, for lazily-started coroutines, or `suspend_never`, for eagerly-started coroutines.
- when `co_await promise.initial_suspend()` resumes, starts executing the body of the coroutine

```c++
template<typename T>
struct generator
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro;
    T get_value() { return coro.promise().current_value; }

    struct promise_type
    {
        T current_value;
        promise_type() = default;
        ~promise_type() = default;

        auto get_return_object()
        { return generator{ handle_type::from_promise(*this)}; }
        std::suspend_never initial_suspend() { return {}; } // eagerly-started
        // ...
    };
}
```

When a coroutine reaches a suspension point
- the return object obtained earlier is returned to the caller/resumer, after implicit conversion to the return type of the coroutine, if necessary.

When a coroutine reaches the `co_return` statement, it performs the following:
- calls `promise.return_void()` for
  - `co_return;`
  - `co_return expr` where `expr` has type `void`
  - falling off the end of a void-returning coroutine. The behavior is undefined if the `Promise type` has no `Promise::return_void()` member function in this case.
- or calls `promise.return_value(expr)` for `co_return expr` where `expr` has non-`void` type
- destroys all variables with automatic storage duration in reverse order they were created.
- calls `promise.final_suspend()` and `co_await`'s the result.

If the coroutine ends with an uncaught exception, it performs the following:
- catches the exception and calls `promise.unhandled_exception()` from within the catch-block
- calls `promise.final_suspend()` and `co_await`'s the result (e.g. to resume a continuation or publish a result). It's undefined behavior to resume a coroutine from this point.

```c++
template<typename T>
struct generator
{
    // ...
    struct promise_type
    {
        // ...
        void return_void() {}
        ??? return_value(expr) { ??? }  // ????
        std::suspend_never final_suspend() { return {}; }
        void unhandled_exception() {}
    };
}
```

When the coroutine state is destroyed either because it terminated via `co_return` or uncaught exception, or because it was destroyed via its handle, it does the following:
- calls the destructor of the promise object.
- calls the destructors of the function parameter copies.
- calls operator delete to free the memory used by the coroutine state
- transfers execution back to the caller/resumer.

