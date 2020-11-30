#include <string>
#include <vector>
#include <cassert>
#include <iostream>

struct Record
{
    std::string name {};
    int phone {0};
};

constexpr auto less = [](const Record& a, const Record& b)
{ return a.name < b.name; };

int main()
{
    using namespace std::literals;
    std::vector<Record> vr0
    {
        {"a"s, 1111}, {"a"s, 1112},
        {"b"s, 2222},
        {"c"s, 3333}, {"c"s, 3334},
        {"d"s, 4444}
    };

    auto er = equal_range(vr0.begin(), vr0.end(),
        Record{"c"s}, less);
    for (auto p = er.first; p != er.second; p++)
    { std::cout << p->name << ':' << p->phone << '\n'; }


    auto [first, last] = equal_range(vr0.begin(), vr0.end(),
        Record{"c"s}, less);
    for (auto p = first; p != last; p++)
    { std::cout << p->name << ':' << p->phone << '\n'; }
}
