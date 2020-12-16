#include <cassert>
#include <iostream>

struct length_in_km
{
    constexpr explicit length_in_km(double d) : val{d} {}
    constexpr double get_value() { return val; }
private:
    double val;
};

struct length_in_mile
{
    constexpr explicit length_in_mile(double d) : val{d} {}
    constexpr double get_value() { return val; }
    constexpr operator length_in_km() { return length_in_km(1.609344 * val); }
private:
    double val;
};

constexpr length_in_km marks[] { length_in_mile(2.3), length_in_mile(0.76) };

int main()
{
    // constexpr double len = mark1->get_value();
    // std::cout << mark1.get_value() << '\n';
    // static_assert( 3.7 < mark1.get_value() );
}