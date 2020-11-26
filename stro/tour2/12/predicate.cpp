#include <utility>
#include <string>
#include <map>
#include <cassert>

struct Greater_than
{
    int val{0};
    Greater_than(int v) : val{v} {}
    bool operator()(std::pair<const std::string, int>& v)
    { return v.second > val; }
};

int main()
{
    std::map<std::string, int> map{{"a", 1}, {"b", 2}, {"c", 3}};
    // using a function object
    auto p1 = find_if(map.begin(), map.end(), Greater_than{1});
    assert(p1->second == 2);
    // using a lambda
    auto p2 = find_if(map.begin(), map.end(), [](const auto& r){ return r.second > 1; });
    assert(p2->second == 2);
}
