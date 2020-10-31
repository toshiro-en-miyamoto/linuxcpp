#include <iostream>

template<typename T, size_t SZ>
void print(T* const ptr) {
    for (auto i=0; i!=SZ; i++)
        std::cout << ptr[i];
    std::cout << '\n';
}

void increment() {
    int v[] {0,1,2,3,4,5,6,7,8,9};
    constexpr size_t size = sizeof v/sizeof(int);

    for (auto i=0; i!=size; i++)
        v[i]++;
    print<int, size>(v);

    for (auto x : v)
        x++;
    print<int, size>(v);

    for (auto& x : v)
        x++;
    print<int, size>(v);
}

int main() {
    increment();
int x {7};
int& r {x};    // bind r to x (r refers to x)
r = 14;         // assign to whatever r refers to
std::cout << x << '\n';
}