#include <iostream>

struct Vector_in_struct {
    double *elem;
    int sz;
};

class Vector {
public:
    Vector(int s) :elem { new double[s] }, sz {s} {}
    double& operator[](int i) { return elem[i]; }
    int size() { return sz; }
private:
    double *elem;
    int sz;
};

int main() {
    constexpr int size {4};
    Vector v {size};
    std::cout << "Enter " << size << " doubles:\n";
    // error: range-for statements require 'begin()'
    // for (auto& e : v) {}
    for (int i=0; i!=size; i++)
        std::cin >> v[i];
    std::cout << '\n';
    for (int i=0; i!=size; i++)
        std::cout << v[i] << ' ';
    std::cout << '\n';
}