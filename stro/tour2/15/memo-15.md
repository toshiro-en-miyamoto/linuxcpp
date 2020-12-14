# Concurrency

This section briefly gives examples of the main standard-library concurrency support facilities: `threads`, `mutexes`, `lock()` operations, `packaged_tasks`, and `future`s. These features are built directly upon what operating systems offer and do not incur performance penalties compared with those.

## Tasks and `thread`s (§15.2)

A task to be executed concurrently with other tasks is launched by constructing a `std::thread` (found in `<thread>`) with the task as its argument. A task is a function or a function object:

```c++
void f();                 // function

struct F {                // function object
    void operator()();
}

void user()
{
    thread t1 {f};       // f() executes in separate thread
    thread t2 {F()};     // F()() executes in separate thread

    t1.join();           // wait for t1
    t2.join();           // wait for t2
}
```

To `join()` a thread means "wait for the thread to terminate."

When defining tasks of a concurrent program, our aim is to keep tasks completely separate except where they communicate in simple and obvious ways. The simplest way of thinking of a concurrent task is as a function that happens to run concurrently with its caller. For that to work, we just have to pass arguments, get a result back, and make sure that there is no use of shared data in between (no data races).

## Passing Arguments (§15.3)

Typically, a task needs data to work upon. We can easily pass data (or pointers or references to the data) as arguments.

```c++
void f(std::vector<double>& v);

struct F
{
    std::vector<double>& v;
    F(std::vector<double>& vv) : v{vv} {}
    void operator()();
}

void use()
{
    std::vector<double> some_vec{1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::vector<double> vec2{10, 11, 12, 13, 14};

    thread t1 {f, ref(some_vec)};
    thread t2 {F{vec2}};

    t1.join();
    t2.join();
}
```

The initialization with `{f, ref(some_vec)}` uses a thread variadic template constructor that can accept an arbitrary sequence of arguments. The `ref()` is a type function from `<functional>` that unfortunately is needed to tell the variadic template to treat `some_vec` as a reference, rather than as an object. Without that `ref()`, `some_vec` would be passed by value.

## Sharing Data (§15.5)

Sometimes tasks need to share data. In that case, the access has to be synchronized so that at most one task at a time has access.

The fundamental element of the solution is a `mutex`, a "mutual exclusive object." A thread acquires a `mutex` using a `lock()` operation:

```c++
mutex m; // controlling mutex
int sh;  // shared data

void f()
{
    scoped_lock lck {m};        // acquire mutex
    sh += 7;
} // release mutex implicitly
```

The `scoped_lock`’s constructor acquires the mutex (through a call `m.lock()`). If another thread has already acquired the mutex, the thread waits ("blocks") until the other thread completes its access. Once a thread has completed its access to the shared data, the `scoped_lock` releases the mutex (with a call `m.unlock()`). When a mutex is released, threads waiting for it resume executing ("are woken up"). The mutual exclusion and locking facilities are found in `<mutex>`.

Use of resource handles, such as `scoped_lock` and `unique_lock`, is simpler and far safer than explicitly locking and unlocking `mutex`es.

[...] some people are convinced that sharing must be more efficient than copying arguments and returns. That can indeed be so when large amounts of data are involved, but locking and unlocking are relatively expensive operations. On the other hand, modern machines are very good at copying data, especially compact data, such as `vector` elements. So don’t choose shared data for communication because of “efficiency” without thought and preferably not without measurement.

One of the most common ways of sharing data is among many readers and a single writer. This "eader-writer lock" idiom is supported be `shared_mutex`. A reader will acquire the mutex "shared" so that other readers can still gain access, whereas a writer will demand exclusive access.

```c++
shared_mutex mx;

void reader()
{
    shared_lock lck {mx};
    // ... read ...
}
void writer()
{
    unique_lock lck {mx};
    // ... write ...
}
```

## Waiting for Events (§15.6)

Sometimes, a `thread` needs to wait for some kind of external event, such as another `thread` completing a task or a certain amount of time having passed.

The basic support for communicating using external events is provided by `condition_variables` found in `<condition_variable>`. A `condition_variable` is a mechanism allowing one `thread` to wait for another. In particular, it allows a `thread` to wait for some condition (often called an event) to occur as the result of work done by other `thread`s.

```c++
class Message {
    // ...
};

std::queue<Message> mqueue;
std::condition_variable mcond;
std::mutex mmutex;

void consumer()
{
    while (true)
    {
        std::unique_lock lck{mmutex};
        mcond.wait(lck, []{ return !mqueue.empty(); });

        auto m = mqueue.first();
        mqueue.pop();
        lck.unlock();
        // ... process m ...
    }
}

void provider()
{
    while (true)
    {
        Message m;
        // ... fill the message ...
        std::scoped_lock lck{mmutex};
        mqueue.push(m);
        mcond.notify_one()
    }   // release lock (at end of scope)
}
```

I explicitly protect the operations on the `queue` and on the `condition_variable` with a `unique_lock` on the `mutex`.

Waiting on `condition_variable` releases its lock argument until the wait is over (so that the `queue` is non-empty) and then reacquires it. The explicit check of the condition, here `!mqueue.empty()`, protects against waking up just to find that some other task has "gotten there first" so that the condition no longer holds.

I used a `unique_lock` rather than a `scoped_lock` for two reasons:

- We need to pass the lock to the `condition_variable`’s `wait()`. A `scoped_lock` cannot be moved, but a `unique_lock` can be.
- We want to unlock the `mutex` protecting the `condition_variable` before processing the message. A `unique_lock` offers operations, such as `lock()` and `unlock()`, for low-level control of synchronization.

## Communicating Tasks (§15.7)

The standard library provides a few facilities to allow programmers to operate at the conceptual level of tasks (work to potentially be done concurrently) rather than directly at the lower level of threads and locks:

- `future` and `promise` for returning a value from a task spawned on a separate `thread`
- `packaged_task` to help launch tasks and connect up the mechanisms for returning a result
- `async()` for launching of a task in a manner very similar to calling a function

These facilities are found in `<future>`.

### `future` and `promise`

The main purpose of a `promise` is to provide simple "put" operations (called `set_value()` and `set_exception()`) to match `future`’s `get()`.

```c++
// a task: place the result in px
void f(std::promise<X>& px)
{
    // ...
    try {
        X res;
        // ... compute a value for res ...
        px.set_value(res);
    } catch (...) {
        px.set_exception(current_exception());
    }
}
```

The `current_exception()` refers to the caught exception.

To deal with an exception transmitted through a `future`, the caller of `get()` must be prepared to catch it somewhere.

```c++
// a task: get the result from fx
void g(future<X>& fx)
{
    // ...
    try {
        X v = fx.get();
        // if necessary, wait for the value to get computed
        // ... use v ...
    }
    catch (...) {
        // ... handle error ...
    }
}
```

### `packaged_task`

A `packaged_task` provides wrapper code to put the return value or exception from the task into a `promise`.

```c++
double accum(double* beg, double* end, double init)
{
    return accumulate(beg, end, init);
}
double comp2(vector<double>& v)
{
    using Task_type = double(double*,double*,double);

    std::packaged_task<Task_type> pt0 {accum};
    std::packaged_task<Task_type> pt1 {accum};

    std::future<double> f0 {pt0.get_future()};
    std::future<double> f1 {pt1.get_future()};

    double* first  = &v[0];
    double* middle = first + v.size()/2;
    double* last   = first + v.size();
    thread t1 {move(pt0), first, middle, 0};
    thread t2 {move(pt1), middle, last, 0};

    // ...
    return f0.get() + f1.get();
}
```

The `move()` operations are needed because a `packaged_task` cannot be copied. The reason that a `packaged_task` cannot be copied is that it is a resource handle: it owns its `promise` and is (indirectly) responsible for whatever resources its task may own.

### `async()`

```c++
double comp4(vector<double>& v)
{
    std::future<double> f0 {pt0.get_future()};
    std::future<double> f1 {pt1.get_future()};

    auto v0 = &v[0];
    auto sz = v.size();

    auto f0 = async(accum, v0, v0+sz/4, 0.0);
    auto f1 = async(accum, v0+sz/4, v0+sz/2, 0.0);
    auto f2 = async(accum, v0+sz/2, v0+sz*3/4, 0.0);
    auto f3 = async(accum, v0+sz*3/4, v0+sz, 0.0);
    // ...
    return f0.get() + f1.get() + f2.get() + f3.get();
}
```

Using `async()`, you don’t have to think about threads and locks. Instead, you think just in terms of tasks that potentially compute their results asynchronously.

It is rarely necessary to manually parallelize a standard-library algorithm, such as `accumulate()`, because the parallel algorithms, such as `reduce(par_unseq, /*...*/)`, usually do a better job at that.

## Back to Basics: Concurrency. Arthur O'Dwyer. Cppcon 2020

In conclusion (slide 51):
- Unprotected data races are UB
  - Use `std::mutex` to protect *all* accesses (both reads and writes)
- Thread-safe static initialization is your friend
  - Use `std::once_flag` only when the initializee is non-static
- `mutext` + `condition_variable` are best friends
- C++20 gives us "couting" primitives like sampphore and latch
- But if your program is **fundamentally multithreaded**, look for higher-level facilities: `premise`/`future`, coroutines, ASIO, TBB
