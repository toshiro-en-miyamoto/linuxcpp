#include <cassert>
#include <vector>
#include <string>

template<typename... T>
int sum_variadic_fold(T... v)
{
    return (v + ... + 0);
}

template<typename T, typename... Ts>
std::vector<T> to_vector(Ts&&... ts)
{
    std::vector<T> res;
    (res.push_back(ts), ...);
    return res;
}

int main()
{
    using namespace std::literals::string_literals;
    assert(sum_variadic_fold(1, 2, 3, 4) == 10);

    std::vector hello{"Hello"s, "world"s};
    auto v = to_vector<std::string>("Hello"s, "world"s);
    assert(hello == v);
}