#include "Vector.hpp"
#include <cassert>

int main() {
    Vector v {1, 2, 3, 4, 5};
    double a = 0;
    for (size_t i = 0; i < v.size(); i++)
        a += v[i];
    assert(a == 15);
}