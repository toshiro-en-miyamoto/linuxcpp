
constexpr int counter = 10'000'000;

void unlocked_increment(int& result)
{
    for (auto c = counter; c!=0; c--)
        result += 1;
}

struct unlocked_increment_t
{
    int& result;
    unlocked_increment_t(int& r) : result{r} {}
    void operator()()
    {
        for (auto c = counter; c!=0; c--)
            result += 1;
    }
};

#include <mutex>

std::mutex m;

void locked_increment(int& result)
{
    for (auto c = counter; c!=0; c--)
    {
        std::scoped_lock lck{m};
        result++;
    }
}

struct locked_increment_t
{
    int& result;
    locked_increment_t(int& r) : result{r} {}
    void operator()()
    {
        for (auto c = counter; c!=0; c--)
        {
            std::scoped_lock lck{m};
            result++;
        }
    }
};

#include <thread>
#include <functional>
#include <cassert>

void unlocked_concurrent_threads()
{
    int result {0};
    std::thread thread1{unlocked_increment, std::ref(result)};
    std::thread thread2{unlocked_increment_t{result}};
    thread1.join();
    thread2.join();
    assert( result < 2*counter );
}

void unlocked_serial_threads()
{
    int result {0};
    std::thread thread1{unlocked_increment, std::ref(result)};
    thread1.join();
    std::thread thread2{unlocked_increment_t{result}};
    thread2.join();
    assert( result == 2*counter );
}

void unclocked_independent_threads()
{
    int result1 {0};
    int result2 {0};
    std::thread thread1{unlocked_increment, std::ref(result1)};
    std::thread thread2{unlocked_increment_t{result2}};
    thread1.join();
    thread2.join();
    assert( result1 + result2 == 2*counter );
}

void locked_concurrent_threads()
{
    int result {0};
    std::thread thread1{locked_increment, std::ref(result)};
    std::thread thread2{locked_increment_t{result}};
    thread1.join();
    thread2.join();
    assert( result == 2*counter );
}

void locked_serial_threads()
{
    int result {0};
    std::thread thread1{locked_increment, std::ref(result)};
    thread1.join();
    std::thread thread2{locked_increment_t{result}};
    thread2.join();
    assert( result == 2*counter );
}

#include <chrono>
#include <vector>
#include <algorithm>
#include <utility>

std::pair<int, int> duration_of(void (fp)())
{
    using namespace std::chrono;
    std::vector<milliseconds::rep> durations(100);
    for (auto& d : durations)
    {
        auto t1_start = high_resolution_clock::now();
        fp();
        auto t1_end = high_resolution_clock::now();
        d = duration_cast<milliseconds>(t1_end - t1_start).count();
    }
    return {std::ranges::min(durations), std::ranges::max(durations)};
}

#include <iostream>

int main()
{
    /*
    using namespace std::chrono;
    auto t1_start = high_resolution_clock::now();
    unlocked_concurrent_threads();
    auto t1_end = high_resolution_clock::now();
    auto d1 = duration_cast<milliseconds>(t1_end - t1_start).count();
    assert(3'500 < d1 & d1 < 4'800);
    */
    auto [d1_min, d1_max] = duration_of(unlocked_concurrent_threads);
    std::cout << "unlocked current threads:     "
        << d1_min << ',' << d1_max << '\n';
    auto [d2_min, d2_max] = duration_of(unlocked_serial_threads);
    std::cout << "unlocked_serial_threads:      "
        << d2_min << ',' << d2_max << '\n';
    auto [d3_min, d3_max] = duration_of(unclocked_independent_threads);
    std::cout << "unlocked_independent_threads: "
        << d3_min << ',' << d3_max << '\n';

    auto [d4_min, d4_max] = duration_of(locked_concurrent_threads);
    std::cout << "locked current threads:       "
        << d4_min << ',' << d4_max << '\n';
    auto [d5_min, d5_max] = duration_of(locked_serial_threads);
    std::cout << "locked_serial_threads:        "
        << d5_min << ',' << d5_max << '\n';
}