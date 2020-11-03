// c++ -Wall -std=c++2a -fimplicit-modules -fimplicit-module-maps -c Vector.cpp -Xclang -emit-module-interface -o Vector.pcm

module;
export module Vector;
#include <stdexcept>
#include <cassert>

export class Vector {
public:
    Vector(int s);
    double& operator[](int i);
    int size();
private:
    double* elem;     // elem points to an array of sz doubles
    int sz;
};

Vector::Vector(int s) {
    if (s<=0)
        throw std::length_error("Vector constructor");
    elem = new double[s];
    sz = s;
}

double& Vector::operator[](int i) {
    assert(elem!=nullptr);
    if (i<0 || sz<=i)
        throw std::out_of_range{"Vector::operator[]"};
    return elem[i];
}

int Vector::size() {
    return sz;
}
