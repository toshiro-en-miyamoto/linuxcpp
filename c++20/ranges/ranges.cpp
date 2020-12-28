#include <vector>
#include <ranges>
#include <algorithm>
#include <numeric>
#include <cassert>

void range1()
{
    std::vector<int> series{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int result = 0;
    for (int i : series
        | std::views::filter([](int i){ return i%2 == 0; })
        | std::views::transform([](int i){ return i*2; })
        | std::views::take(4)
    ){
        result += i;
    };
    assert(result == 40);
}

void range2()
{
    std::vector<int> series{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto view = series
        | std::views::filter([](int i){ return i%2 == 0; })
        | std::views::transform([](int i){ return i*2; })
        | std::views::take(4);
    int result = 0;
    std::ranges::for_each(view, [&](int i){ result += i; });
    assert(result == 40);
}

void range3()
{
    std::vector<int> series{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int result = 0;
    std::ranges::for_each( series
        | std::views::filter([](int i){ return i%2 == 0; })
        | std::views::transform([](int i){ return i*2; })
        | std::views::take(4)
        , [&](int i){ result += i; });
    assert(result == 40);
}

void range4()
{
    std::vector<int> series{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> result;
    std::ranges::for_each( series
        | std::views::filter([](int i){ return i%2 == 0; })
        | std::views::transform([](int i){ return i*2; })
        | std::views::take(4)
        , [&](int i){ result.push_back(i); });
    std::vector<int> v{4,8,12,16};
    assert(v == result);
}

void range5()
{
    std::vector<int> series{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto view = series
        | std::views::filter([](int i){ return i%2 == 0; })
        | std::views::transform([](int i){ return i*2; })
        | std::views::take(4)
        | std::views::common;
    std::vector<int> result(view.begin(), view.end());
    std::vector<int> v{4,8,12,16};
    assert(v == result);
}

void range6()
{
    std::vector<int> series{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto view = series
        | std::views::filter([](int i){ return i%2 == 0; })
        | std::views::transform([](int i){ return i*2; })
        | std::views::take(4)
        | std::views::common;
    int result = std::accumulate(view.begin(), view.end(), 0);
    assert(result == 40);
}

#include <iterator>
#include <functional>

// Tristan Brindle, "C++20 Ranges in Practice", CppCon2020
template<std::input_iterator I, std::sentinel_for<I> S,
            typename Init = std::iter_value_t<I>,
            typename Op = std::plus<>>
constexpr Init range_accumulate(I first, S last, Init init = Init(), Op op = Op{})
{
    while (first != last)
    {
        init = std::invoke(op, std::move(init), *first);
        ++first;
    }
    return init;
};

void range7()
{
    std::vector<int> series{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto view = series
        | std::views::filter([](int i){ return i%2 == 0; })
        | std::views::transform([](int i){ return i*2; })
        | std::views::take(4);
    int result = range_accumulate(view.begin(), view.end(), 0);
    assert(result == 40);
}

// Tristan Brindle, "C++20 Ranges in Practice", CppCon2020
template<std::ranges::input_range R,
            typename Init = std::ranges::range_value_t<R>,
            typename Op = std::plus<>>
constexpr Init range_accumulate(R&& range, Init init = Init(), Op op = Op{})
{
    return range_accumulate(
        std::ranges::begin(range), std::ranges::end(range),
        std::move(init), std::move(op));
};

void range8()
{
    std::vector<int> series{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto view = series
        | std::views::filter([](int i){ return i%2 == 0; })
        | std::views::transform([](int i){ return i*2; })
        | std::views::take(4);
    int result = range_accumulate(view, 0);
    assert(result == 40);
}

// Tristan Brindle, "C++20 Ranges in Practice", CppCon2020
// function objects with range adaptors to trim white spaces
inline constexpr
auto trim_front = std::views::drop_while(::isspace);

inline constexpr
auto trim_back = std::views::reverse
                | std::views::drop_while(::isspace)
                | std::views::reverse;

std::string trim_string(const std::string& str)
{
    auto view = str | trim_front | trim_back | std::views::common;
    return std::string(view.begin(), view.end());
}

void range9()
{
    std::string hello{"   Hello    "};
    auto result = trim_string(hello);
    std::string expected{"Hello"};
    assert(result == expected);
}

int main()
{
    range1();
    range2();
    range3();
    range4();
    range5();
    range6();
    range7();
    range8();
    range9();
}