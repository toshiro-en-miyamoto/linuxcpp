#include <cassert>

struct Base {
    // must be polymorphic to use runtime-checked dynamic_cast
    virtual ~Base() {}
};
struct Derived: Base {};

int main() {
    Base* b = new Derived;
    Derived* d = dynamic_cast<Derived*>(b);
    assert(d != nullptr);
}