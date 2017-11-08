#include <sstream>
#include <exception>
#include <iostream>

template<typename Target, typename Source>
Target to(Source arg)
{
    std::stringstream interpreter;
    Target result;

    if(!(interpreter << arg)
        || !(interpreter >> result)
        || !(interpreter >> std::ws).eof())
            throw std::runtime_error{"to<>() failed"};

    std::cout << result << std::endl;
    return result;
}

int main()
try
{
    int i1 = to<int>("123 ");
    int i2 = to<int>("123.4");
}
catch(std::runtime_error e) {
    std::cerr << e.what() << std::endl;
}