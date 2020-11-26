#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <cassert>
#include <iostream>

struct Entry
{
    std::string name{};
    int phone{0};
};

std::ostream& operator<<(std::ostream& os, const Entry& e)
{ return os << "{" << e.name << ", " << e.phone << "}"; }

bool operator<(const Entry& a, const Entry& b)
{ return a.name < b.name; }

bool operator==(const Entry& a, const Entry& b)
{ return (a.name == b.name) & (a.phone == b.phone); }

void f1(std::vector<Entry>& v, std::list<Entry>& l)
{
    std::sort(v.begin(), v.end());
    unique_copy(v.begin(), v.end(), l.begin());
}

std::list<Entry> f2(std::vector<Entry>& v)
{
    std::sort(v.begin(), v.end());
    std::list<Entry> l;
    unique_copy(v.begin(), v.end(), back_inserter(l));
    return l;
}

int main()
{
    std::vector<Entry> v1{Entry{"x", 1234}, Entry{"y", 3456}};

    std::list<Entry> l1(v1.size());
    f1(v1, l1);
    std::vector<Entry> v2{l1.begin(), l1.end()};
    assert(v1 == v2);

    std::list<Entry> l2{f2(v1)};
    assert(l1 == l2);
}