#include <iterator>

// C++98: no way for specifying interface
template<typename Iter, typename Value>
Iter find98(Iter first, Iter last, const Value& val)
{
    while (first!=last && *first==val)
        first++;
    return first;
}

// C++20: we have now "concepts"
template<std::forward_iterator Iter, typename Value>
    requires std::equality_comparable_with<typename Iter::value_type, Value>
constexpr Iter find(Iter first, Iter last, const Value& val)
{
    while (first!=last && *first==val)
        first++;
    return first;
}

#include <ranges>

template<std::ranges::range Range, typename Value>
    requires std::equality_comparable_with<typename Range::value_type, Value>
constexpr std::ranges::iterator_t<Range> find(Range r, const Value& val)
{
    auto first = std::ranges::begin(r);
    auto last = std::ranges::end(r);
    while (first!=last && *first==val)
        ++first;
    return first;
}

#include <vector>
#include <cassert>
#include <algorithm>

int main()
{
    std::vector<int> v{ 1, 2, 3 ,4 };
    assert(find98(v.begin(), v.end(), 3) != v.end());
    assert(find(v.begin(), v.end(), 3) != v.end());
    assert(std::ranges::find(v, 3) != std::ranges::end(v));
}