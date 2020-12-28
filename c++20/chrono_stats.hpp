#include <ranges>
#include <iterator>
#include <functional>

// Tristan Brindle, "C++20 Ranges in Practice", CppCon2020
template<std::input_iterator I, std::sentinel_for<I> S,
            typename Init = std::iter_value_t<I>,
            typename Op = std::plus<>>
inline constexpr Init range_accumulate(I first, S last, Init init = Init(), Op op = Op{})
{
    while (first != last)
    {
        init = std::invoke(op, std::move(init), *first);
        ++first;
    }
    return init;
};

// Tristan Brindle, "C++20 Ranges in Practice", CppCon2020
template<std::ranges::input_range R,
            typename Init = std::ranges::range_value_t<R>,
            typename Op = std::plus<>>
inline constexpr Init range_accumulate(R&& range, Init init = Init(), Op op = Op{})
{
    return range_accumulate(
        std::ranges::begin(range), std::ranges::end(range),
        std::move(init), std::move(op));
};

#include <chrono>
#include <algorithm>

template<typename T, int i>
    requires std::same_as<T, std::chrono::nanoseconds>
          || std::same_as<T, std::chrono::microseconds>
          || std::same_as<T, std::chrono::milliseconds>
struct chrono_stats_t
{
    using type_name = T;
    static const int iteration = i;
    typename T::rep min;
    typename T::rep avg;
    typename T::rep max;
};

template<typename T, int iteration = 1'000>
inline chrono_stats_t<T, iteration> chrono_stats_of(void (fp)())
{
    using namespace std::chrono;
    std::vector<typename T::rep> durations(iteration);
    for (auto& d : durations)
    {
        auto t1_start = high_resolution_clock::now();
        fp();
        auto t1_end = high_resolution_clock::now();
        d = duration_cast<T>(t1_end - t1_start).count();
    }
    return {std::ranges::min(durations),
            range_accumulate(durations)/iteration,
            std::ranges::max(durations)};
}





