#include <concepts>
#include <vector>
// #include <utility>
#include <tuple>

template <typename T>
    requires std::integral<T> || std::floating_point<T>
struct matrix
{
    std::vector<std::vector<T>> val;
};

template <typename T>
auto SVD11(matrix<T>& m) -> std::tuple<matrix<T>, std::vector<T>, matrix<T>>
{
    matrix<T> m1, m2;
    vector<T> v;
    // ...
    return make_tuple(m1, v, m2);
}

template <typename T>
auto SVD17(matrix<T>& m) -> std::tuple<matrix<T>, std::vector<T>, matrix<T>>
{
    matrix<T> m1, m2;
    vector<T> v;
    // ...
    return {m1, v, m2};
}

void use()
{
    matrix<int> m1, m2;
    std::vector<int> v;
    tie(m1, v, m2) = SVD11(m1);
    auto [m1, v, m2] = SVD11(m1);
    auto [m1, v, m2] = SVD17(m1);
}
