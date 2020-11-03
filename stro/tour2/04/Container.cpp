#include "Container.hpp"
#include <cassert>

double accum(Container& c) {
    const size_t sz = c.size();
    double a = 0;
    for (size_t i = 0; i < sz; i++)
        a += c[i];
    return a;
}