#include <memory>
#include <cassert>

struct X
{
    int value {0};
    X() {}
    X(int v) : value {v} {}
    X(const X& x) : value { x.value } {}
    X(X&& x) : value { x.value } {}
    X& operator=(X&& x) { value = x.value; return *this; }
    ~X() {}
};

void f1()
{
    auto p = std::make_unique<X>(2);
    auto q = move(p);
    assert(p == nullptr);
}

void swap()
{
    auto p = std::make_unique<X>(1);
    auto q = std::make_unique<X>(2);

    auto tmp = move(p);
    p = move(q);
    q = move(tmp);
    assert(p->value == 2);
    assert(q->value == 1);
}

int main()
{
    f1();
    swap();
}
