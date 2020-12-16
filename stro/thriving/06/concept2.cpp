#include <concepts>

template<typename T>
concept integer = std::same_as<T, int> || std::same_as<T, long> || std::same_as<T, long long>;

constexpr integer auto x1 = 7;
constexpr int x2 = 9;

constexpr integer auto y1 = x1 + x2;
constexpr int y2 = x2 + x1;

constexpr integer auto add(integer auto a, integer auto b)
{
    return a + b;
}

int main()
{
    constexpr auto z1 = add(x1, y1);
    static_assert(23 == z1);
}
