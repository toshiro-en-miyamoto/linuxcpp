#include <vector>
#include <algorithm>
#include <cassert>

int main()
{
    std::vector<int> v1{1, 2, 1, 1, 4, 1};
    std::vector<int> v2;
    unique_copy(v1.begin(), v1.end(), back_inserter(v2));
    std::vector<int> v3{1, 2, 1, 4, 1};
    assert(v2 == v3);
}