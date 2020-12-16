#include <utility>
#include <string>
#include <tuple>
#include <array>
#include <cassert>

using namespace std::literals;

constexpr auto hello{"Hello"sv};

constexpr std::pair<std::string_view, int> p1 {hello, 1};
constexpr auto p2 = make_pair(hello, 1);
constexpr std::pair p3 {hello, 1};

static_assert(p1 == p2);
static_assert(p2 == p3);
static_assert(std::get<0>(p1) == hello);
static_assert(std::get<1>(p1) == 1);

constexpr std::array<std::pair<std::string_view, int>, 3> ap
{
    std::pair<std::string_view, int> {hello, 1},
    make_pair(hello, 1),
    std::pair {hello, 1}
};
static_assert(std::get<0>(ap[2]) == hello);
static_assert(std::get<1>(ap[2]) == 1);

constexpr std::pair<std::string_view, int> fp()
{ return {hello, 1}; }

constexpr std::tuple<int, std::string_view, float> t1 {1, hello, 2.0};
constexpr auto t2 = make_tuple(1, hello, 2.0);
constexpr std::tuple t3 {1, hello, 2.0};
static_assert(t1 == t2);
static_assert(t2 == t3);
static_assert(std::get<0>(t1) == 1);
static_assert(std::get<1>(t1) == hello);
static_assert(std::get<2>(t1) == 2.0);

constexpr std::array<std::tuple<int, std::string_view, float>, 3> at
{
    std::tuple<int, std::string_view, float> {1, hello, 2.0},
    make_tuple(1, hello, 2.0),
    std::tuple {1, hello, 2.0}
};
static_assert(std::get<0>(at[2]) == 1);
static_assert(std::get<1>(at[2]) == hello);
static_assert(std::get<2>(at[2]) == 2.0);

constexpr std::tuple<int, std::string_view, float> ft()
{ return {1, hello, 2.0}; }

int main()
{
    auto [p11, p12] = p1;
    assert(p11 == hello);
    auto [p21, p22] = fp();
    assert(p21 == hello);

    auto [t11, t12, t13] = t1;
    assert(t13 == 2.0);
    auto [t21, t22, t23] = ft();
    assert(t23 == 2.0);
}
