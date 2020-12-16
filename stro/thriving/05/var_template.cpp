template<typename T>
struct pi
{ static constexpr T value = T(3.1415); };   // class template

template<typename T>
constexpr T pi_func() { return T(3.1415); };   // function template

template<typename T>
constexpr T pi_var = T(3.1415);   // variable template

template<typename T>
constexpr T circular_area(T pi, T r)
{ return pi * r * r; }

int main()
{
    constexpr auto ac1 = circular_area(pi<int>::value, 1);
    static_assert(3 == ac1);
    constexpr auto ac2 = circular_area(pi<double>::value, 1.0);
    static_assert(3.1414 < ac2 & ac2 < 3.1416);

    constexpr auto af1 = circular_area(pi_func<int>(), 1);
    static_assert(3 == af1);
    constexpr auto af2 = circular_area(pi_func<double>(), 1.0);
    static_assert(3.1414 < af2 & af2 < 3.1416);

    constexpr auto av1 = circular_area(pi_var<int>, 1);
    static_assert(3 == av1);
    constexpr auto av2 = circular_area(pi_var<double>, 1.0);
    static_assert(3.1414 < av2 & av2 < 3.1416);
}