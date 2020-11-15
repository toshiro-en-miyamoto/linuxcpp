#include <vector>
#include <ranges>
#include <cassert>

int main()
{
    std::vector ints { 1, 2, 3, 4 };
    auto even = [](int i) { return 0 == i % 2; };
    auto squre = [](int i) { return i * i; };

    int accum = 0;
    for (auto i : ints | std::views::filter(even) )
    {
        accum += i;
    }
    assert(accum == 6);
}