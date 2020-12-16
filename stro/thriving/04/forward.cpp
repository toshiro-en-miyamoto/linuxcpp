#include <iostream>
#include <memory>

struct A {
    A(int&& n) { std::cout << "rvalue overload, n=" << n << "\n"; }
    A(int& n)  { std::cout << "lvalue overload, n=" << n << "\n"; }
};

template <typename T>
struct B {
    B(T&& n) { std::cout << "rvalue overload, n=" << n << "\n"; }
    B(T& n)  { std::cout << "lvalue overload, n=" << n << "\n"; }
};

template<class T, class U>
std::unique_ptr<T> make_unique1(U&& u)
{
    return std::unique_ptr<T>(new T(std::forward<U>(u)));
}

int main()
{
    auto p1 = make_unique1<A>(2);
    int i = 1;
    auto p2 = make_unique1<A>(i);

    // auto p3 = make_unique1<B>(2);
}