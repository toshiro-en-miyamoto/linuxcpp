#include <vector>

#include <cassert>

template<typename T>
class Vector : public std::vector<T>
{
public:
    T& operator[](int i) { return std::vector<T>::at(i); }
    const T& operator[](int i) const { return std::vector<T>::at(i); }
};

int main()
{
    std::vector<int> v1{1, 2, 3, 4};
    int a1 = 0;
    for (int i = 0; i <= v1.size(); i++)
        a1 += v1[i];
    assert(a1 == 10);  // this will be executed, it happened to be 'true'

    Vector<int> v2;
    for (int i = 0; i < v1.size(); i++)
        v2.push_back(v1[i]);
    int a2 = 0;
    for (int i = 0; i <= v2.size(); i++)
        a2 += v2[i];
        // terminate called after throwing an instance of 'std::out_of_range'
        //  what():  vector::_M_range_check: __n (which is 4) >= this->size() (which is 4)
    assert(a2 == 10);  // this will not be executed
}