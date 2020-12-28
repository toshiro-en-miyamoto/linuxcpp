#include <string>
#include <optional>
#include <cassert>

class customer
{
public:
    struct number
    {
        std::string major;
        std::optional<std::string> minor;
    };
    class name
    {
    public:
        name(std::u8string given, std::u8string family,
            std::optional<std::u8string> middle = std::nullopt
            ) :
                given_{given},
                family_{family},
                middle_{middle}
            {
                assert(given.size() > 0);
                assert(family.size() > 0);
                if (middle) assert(middle.value().size() > 0);
            }
        std::u8string given()  const { return given_;  }
        std::u8string family() const { return family_; }
        std::optional<std::u8string> middle() const { return middle_; }
    private:
        const std::u8string given_;
        std::optional<const std::u8string> middle_;
        const std::u8string family_;
    };
};