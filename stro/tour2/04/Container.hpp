#include <algorithm>

class Container {
public:
    virtual double& operator[](size_t) = 0;
    virtual size_t size() const = 0;
    virtual ~Container() {}
};

double accum(Container&);