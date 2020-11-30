#include <chrono>
#include <cassert>

inline auto clock_now()
{
    using namespace std::chrono;
    return high_resolution_clock::now();
}

template<typename T>
inline auto duration_microseconds(T start, T end)
{
    using namespace std::chrono;
    return duration_cast<microseconds>(end-start).count();
}

int main()
{
    int a[1'000'000];
    auto t0 = clock_now();
    for (auto i : a) ;
    auto t1 = clock_now();
    auto d1 = duration_microseconds(t0, t1);
    assert(3'000 < d1 && d1 < 9'000);
}