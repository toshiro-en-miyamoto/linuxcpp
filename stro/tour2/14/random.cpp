#include <cstddef>
#include <limits>
#include <vector>
#include <random>
#include <numeric>
#include <cassert>

int main()
{
    constexpr int d_min {1};
    constexpr int d_max {6};
    std::uniform_int_distribution<> distri {d_min, d_max};
    std::default_random_engine engine {};

    constexpr std::size_t cnt {1'000'000};
    static_assert(d_max * cnt < std::numeric_limits<int>::max());
    std::vector<int> v(cnt);

    for (auto& p : v)
    { p = distri(engine); }

    int sum = std::accumulate(v.begin(), v.end(), 0);
    float avg = static_cast<float>(sum) / cnt;
    assert(3.49 < avg && avg < 3.51);
}