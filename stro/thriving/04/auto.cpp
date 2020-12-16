template <typename T>
struct vector98
{
    struct iterator {};
    iterator begin() const;
    iterator end() const;
};

template <typename T>
vector98<T>::iterator vector98<T>::begin() const
{ return iterator{}; }

template <typename T>
vector98<T>::iterator vector98<T>::end() const
{ return iterator{}; }

template <typename T>
struct vector11
{
    struct iterator {};
    auto begin() const -> iterator;
    auto end() const -> iterator;
};

template <typename T>
auto vector11<T>::begin() const -> iterator
{ return iterator{}; }

template <typename T>
auto vector11<T>::end() const -> iterator
{ return iterator{}; }

int main()
{
}
