#include <string>
#include <algorithm>
#include <vector>
#include <list>
#include <cassert>

bool has_c1(const std::string& s, char c)
{
    auto p = find(s.begin(), s.end(), c);
    return p != s.end();
}

bool has_c2(const std::string& s, char c)
{ return find(s.begin(), s.end(), c) != s.end(); }

template<typename C, typename E>
bool has_c3(const C& c, E e)
{ return find(c.cbegin(), c.cend(), e) != c.cend(); }

std::vector<std::string::iterator> find_c1(std::string& s, char c)
{
    std::vector<std::string::iterator> v;
    for (auto p = s.begin(); p != s.end(); p++)
    { if (*p == c) v.push_back(p); }
    return v;
}

template<typename C, typename E>
std::vector<typename C::iterator> find_c2(C& c, E e)
{
    std::vector<typename C::iterator> v;
    for (auto p = c.begin(); p != c.end(); p++)
    { if (*p == e) v.push_back(p); }
    return v;
}

template<typename T>
using Iterator = typename T::iterator;

template<typename C, typename E>
std::vector<Iterator<C>> find_c3(C& c, E e)
{
    std::vector<Iterator<C>> v;
    for (auto p = c.begin(); p != c.end(); p++)
    { if (*p == e) v.push_back(p); }
    return v;
}

template<typename T>
using Const_iterator = typename T::const_iterator;

template<typename C, typename E>
std::vector<Const_iterator<C>> find_c4(const C& c, E e)
{
    std::vector<Const_iterator<C>> v;
    for (auto p = c.cbegin(); p != c.cend(); p++)
    { if (*p == e) v.push_back(p); }
    return v;
}

int main()
{
    using namespace std::string_literals;
    auto s{ "tactics"s };
    assert(has_c1(s, 'c'));
    assert(has_c2(s, 'c'));

    int a{0};
    for (auto p : find_c1(s, 'c'))
    { if (*p == 'c') a++; }
    assert(a == 2);

    a = 0;
    for (auto p : find_c2(s, 'c'))
    { if (*p == 'c') a++; }
    assert(a == 2);

    a = 0;
    assert(has_c3(s, 'a'));
    for (auto p : find_c3(s, 'c'))
    { if (*p == 'c') a++; }
    assert(a == 2);
    a = 0;
    for (auto p : find_c4(s, 'c'))
    { if (*p == 'c') a++; }
    assert(a == 2);

    std::list<int> li{1, 2, 1, 4};
    assert(has_c3(li, 2));
    a = 0;
    for (auto p : find_c3(li, 1))
    { if (*p == 1) a++; }
    assert(a == 2);
    a = 0;
    for (auto p : find_c4(li, 1))
    { if (*p == 1) a++; }
    assert(a == 2);

    std::vector<std::string> vs{"am", "pm", "am"};
    assert(has_c3(vs, "pm"s));
    a = 0;
    for (auto p : find_c3(vs, "am"))
    { if (*p == "am") a++; }
    assert(a == 2);
    a = 0;
    for (auto p : find_c4(vs, "am"))
    { if (*p == "am") a++; }
    assert(a == 2);
}
