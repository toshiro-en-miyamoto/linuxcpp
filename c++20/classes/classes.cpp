#include "classes.hpp"

customer::name name()
{
    using namespace std::literals;
    customer::name name1(u8"Jon"s, u8"Doe"s);
    assert(!name1.middle().has_value());
    assert(u8""s == name1.middle().value_or(u8""s));

    customer::name name2(u8"Jane"s, u8"Doe"s, std::make_optional(u8"C++"s));
    assert(name2.middle());  // equivalent to has_value()

    return name2;
}

int main()
{
    name();
}