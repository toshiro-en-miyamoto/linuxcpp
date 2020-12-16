#include <coroutine>
#include <cstdlib>
#include <iostream>

template<typename T>
struct generator
{
    struct promise_type;
    using handle_type = std::coroutine_handle<promise_type>;
    handle_type coro;
    T get_value() { return coro.promise().current_value; }

    generator(handle_type h) : coro{h} {}
    ~generator() { if(coro) coro.destroy(); }
    generator(const generator&) =delete;
    generator& operator=(const generator&) =delete;
    generator(generator&& g) noexcept : coro{g.coro}
    { g.coro = nullptr; }
    generator& operator=(generator&& g) noexcept
    {
        coro = g.coro;
        g.coro = nullptr;
        return *this;
    }

    struct promise_type
    {
        T current_value;
        promise_type() =default;
        ~promise_type() =default;
        std::suspend_always initial_suspend() { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        auto get_return_object()
        { return generator{handle_type::from_promise(*this)}; }
        std::suspend_always yield_value(const T value)
        {
            current_value = value;
            return {};
        }
        void return_void() {}
        void unhandled_exception() {}
    };    
};

generator<int> fibonacci() // generate 0,1,1,2,3,5,8,13 ...
{
    int a = 0;
    int b = 1;
    while (true)
    {
        // n: 1 2 3 5 8 13 21 33 54
        // y: 0 1 1 2 3  5  8 13 21
        // a: 1 1 2 3 5  8 13 21 33
        // b: 1 2 3 5 8 13 21 33 54
        int next = a + b;
        co_yield a;
        a = b;
        b = next;
    }
}

int main()
{
    auto f = fibonacci();
    for (int i = 0; i < 10; i++)
        std::cout << f.get_value() << ' ';
    std::cout << '\n';
}