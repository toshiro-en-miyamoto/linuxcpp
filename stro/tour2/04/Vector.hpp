#include <algorithm>
#include <initializer_list>

class Vector {
    double *elem;
    size_t sz;
public:
    Vector(std::initializer_list<double> list)
      : elem { new double[list.size()] },
        sz { list.size() }
    {
        std::copy(list.begin(), list.end(), elem);
    }

    double& operator[](int i) { return elem[i]; }
    size_t size() const { return sz; }
};
