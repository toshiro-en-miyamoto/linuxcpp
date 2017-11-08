#include <algorithm>
#include <iostream>
#include <vector>

namespace Estd {
    using namespace std;

    template<typename C>
        void sort(C& c)
        { std::sort(c.begin(), c.end()); }
    template<typename C, typename P>
        void sort(C& c, P p)
        { std::sort(c.begin(), c.end(), p); }
}

using namespace Estd;

template<typename T>
void print(const vector<T>& v)
{
    for(auto& x : v)
        cout << x << ' ';
    cout << endl;
}

int main()
{
    std::vector<int> v {7,3,9,4,0,1};

    sort(v);
    print(v);
    sort(v, [](int x, int y){ return x>y; });
    print(v);
    sort(v.begin(), v.end());
    print(v);
    sort(v.begin(), v.end(), [](int x, int y) { return x > y; });
    print(v);
    return 0;
}