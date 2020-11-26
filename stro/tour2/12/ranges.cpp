#include <ranges>
#include <iterator>
#include <vector>
#include <cassert>

template<typename T>
void sort1(T& s) { sort(s.begin(), s.end()); }

template<std::ranges::common_range T>
//    requires std::sortable<T>
void sort2(T& t) { sort(std::ranges::begin(t), std::ranges::end(t)); }

int main()
{
    const std::vector<int> i0{1, 8, 4, 2};
    const std::vector<int> i1{1, 2, 4, 8};

    std::vector<int> i = i0;
    sort1(i);
    assert(i1 == i);

    i = i0;
    sort2(i);
    assert(i1 == i);
}
