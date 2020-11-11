#include <algorithm>

namespace my
{

template<typename T>
class Vector
{
    T* elem = nullptr;
    size_t sz = 0;
public:
    using value_type = T;

    Vector(std::initializer_list<T>);
    explicit Vector(size_t);
    Vector();

    Vector(const Vector<T>&);
    Vector<T>& operator=(const Vector<T>&);

    Vector(Vector<T>&&);
    Vector<T>& operator=(Vector<T>&&);

    inline ~Vector();

    T& operator[](size_t) const;
    inline size_t size() const;
};

template<typename T>
T* begin(Vector<T>&);

template<typename T>
T* end(Vector<T>&);

}