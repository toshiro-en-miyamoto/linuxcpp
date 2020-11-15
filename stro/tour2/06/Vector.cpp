#include "Vector.hpp"
#include <cassert>
#include <stdexcept>
#include <utility>

namespace my
{

template<typename T>
Vector<T>::Vector(std::initializer_list<T> list)
    : elem{ new T[list.size()] }
    , sz{ list.size() }
{
    std::copy(list.begin(), list.end(), elem);
}

template<typename T>
Vector<T>::Vector(size_t len)
    : elem{ new T[len] }
    , sz{ len }
{}

template<typename T>
Vector<T>::Vector()
    : elem{ nullptr }
    , sz{ 0 }
{}

template<typename T>
Vector<T>::Vector(const Vector<T>& v)
    : elem{ new T[v.sz] }
    , sz{ v.sz }
{
    std::copy(v.begin(), v.end(), elem);
}

template<typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& v)
{
    T* p = new T[ v.sz ];
    std::copy(v.begin(), v.end(), p);
    delete[] elem;
    elem = p;
    sz = v.sz;
    return *this;
}

template<typename T>
Vector<T>::Vector(Vector<T>&& v)
    : elem{ v.elem }
    , sz{ v.sz }
{
    v.elem = nullptr;
    v.sz = 0;
}

template<typename T>
Vector<T>& Vector<T>::operator=(Vector<T>&& v)
{
    delete[] elem;
    elem = v.elem;
    sz = v.sz;
    v.elem = nullptr;
    v.sz = 0;
    return *this;
}

template<typename T>
Vector<T>::~Vector()
{
    delete[] elem;
}

template<typename T>
T& Vector<T>::operator[](size_t i) const
{
    assert(elem != nullptr);
    if ( i >= sz )
    {
        throw std::out_of_range{ "Vector::operator[]" };
    }
    return elem[i];
}

template<typename T>
size_t Vector<T>::size() const
{
    return sz; 
}

template<typename T>
T* begin(Vector<T>& v)
{
    return v.size() ? &v[0] : nullptr;
}

template<typename T>
T* end(Vector<T>& v)
{
    return v.size() ? &v[0]+v.size() : nullptr;
}

}

// my::Vector<int> create_vector()
auto create_vector()
{
    my::Vector v{ 1.1, 2.2, 3.3, 4.4 };
    return v;
}

template<typename T>
T accum(my::Vector<T>& v)
{
    T acc{ 0 };
    for (const auto& x : v)
    {
        acc += x;
    }
    return acc;
}

int main()
{
    auto v1 = create_vector();
    assert(accum(v1) == 11.0);
}