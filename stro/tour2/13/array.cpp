#include <array>
#include <cstddef>

struct Shape { void draw(){} };
struct Circle : Shape {};

int main()
{
    std::array<Circle, 3> ac1;
    // error: the value of 'sz' is not usable in a constant expression
    // std::size_t sz = 3;
    constexpr std::size_t sz = 3;
    std::array<Circle, sz> ac2;

    Circle a1[10];
    Shape* p1 = a1;
    p1[3].draw();

    std::array<Circle, 10> a2;
    // Shape* p2 = a2;
}