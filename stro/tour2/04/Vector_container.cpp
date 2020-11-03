#include "Container.hpp"
#include "Vector.hpp"
#include <cassert>

class Vector_container :public Container {
    Vector v;
public:
    Vector_container(std::initializer_list<double> list)
      : v { Vector(list) } {}
    ~Vector_container() {}

    double& operator[](size_t i) override { return v[i]; }
    size_t size() const override { return v.size(); }

};

int main() {
    Vector_container v {1, 2, 3, 4};
    assert(accum(v) == 10);
}