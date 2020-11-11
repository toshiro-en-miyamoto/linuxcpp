#include <cassert>

template<typename T>
class less_than
{
    T val;
public:
    less_than(T x) : val { x } {}
    bool operator()(const T& x) const { return x < val; }
};

template<typename C, typename P>
int count(const C& c, P pred)
{
    int cnt = 0;
    for (const auto& v : c)
    {
        if (pred(v)) cnt++;
    }
    return cnt;
}

int main()
{
    int vi[] { 1, 2, 3, 4, 5, 6 };
    less_than lti { 5 };
    assert(count(vi, lti) == 4);
    constexpr int x = 5;
    assert(count(vi, [&](int a){ return a < x;}) == 4);
}
