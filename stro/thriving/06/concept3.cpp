#include <concepts>

template<typename T>
    requires std::integral<T>
constexpr
T f1(T v)
{ return v * v; }

template<std::integral T>
constexpr
T f2(T v)
{ return v * v; }

constexpr
std::integral
auto f3(std::integral auto v)
{ return v * v; }

template<typename T>
    requires std::integral<T> || std::floating_point<T>
constexpr
T f4(T v)
{ return v * v; }

/*
template<(std::integral || std::floating_point) T>
constexpr
T f5(T v)
{ return v * v; }
*/

/*
constexpr
std::integral || std::floating_point
auto f3(std::integral auto v)
{ return v * v; }
*/

int main()
{
    constexpr auto x1 = f1(4);
    static_assert(16 == x1);

    static_assert(16  == f1(4) );
    static_assert(16L == f1(4L));
    static_assert(16  == f2(4) );
    static_assert(16L == f2(4L));
    static_assert(16  == f3(4) );
    static_assert(16L == f3(4L));

    static_assert(16   == f4(4)  );
    static_assert(16.0 == f4(4.0));
}