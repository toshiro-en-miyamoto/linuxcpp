#include <concepts>
#include <memory>
#include <vector>
#include <iostream>

template <std::integral T>
struct X
{
    T value{0};

    X() {}
    X(T t) : value{t} {}
    ~X() { std::cout << " * destrcting... " << value << '\n'; }
};

template <std::integral T>
std::unique_ptr<X<T>> make_unique_X(T t)
{
    return std::unique_ptr<X<T>> { new X{t} };
}

template <std::integral T, T val>
void change_shared_X(std::shared_ptr<X<T>> sp)
{
    sp->value = val;
}

void using_smart_pointers()
{
    std::unique_ptr<X<int>> sp1 { new X{1} };
    auto sp2 { make_unique_X(2) };
    std::shared_ptr<X<int>> sp3 { new X{3} };
    std::shared_ptr<X<int>> sp4 { new X{4} };
    change_shared_X<int, 2>(sp4);
    std::cout << " * sp4: " << sp4->value << '\n';
    change_shared_X<int, 3>(sp4);
    std::cout << " * sp4: " << sp4->value << '\n';
    auto sp5 { std::make_unique<X<int>>(5) };
    auto sp6 { std::make_shared<X<int>>(6) };
    std::cout << " * falling off smart_pointers()...\n";
}

void using_containers()
{
    std::cout << " * starting containers()...\n";
    std::cout << " * defining a local vector...\n";
    std::vector<X<int>> v;
    std::cout << " * pushing back elements...\n";
    for (int j = 0; j < 4; j++)
        v.push_back(X{ 7+j });
    std::cout << " * refering elements...\n";
    for (auto& e : v)
        std::cout << " * " << e.value << '\n';
    std::cout << " * falling off containers()...\n";
}

/**
 * 
 * 
 **/

int main()
{
    using_smart_pointers();
    using_containers();
}
