#include <string>

namespace Chrono {

    class Date {};

    bool operator==(const Date&, const std::string&);
    std::string format(const Date&);

} // namespace Chrono

void f(Chrono::Date d, int i)
{
    std::string s = format(d);
    std::string t = format(i);
}

void f(Chrono::Date d, std::string s)
{
    if(d == s) {
    }
    else if(d == "August 4, 1914") {
    }
}