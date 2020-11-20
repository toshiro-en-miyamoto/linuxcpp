#include <cassert>
#include <string_view>
#include <iostream>
#include <iomanip>

// ❤ (HEAVY BLACK HEART) U+2764, UTF-8: E2 9D A4
// 💕 (TWO HEART) U+1F495, UTF-8: F0 9F 92 95, UTF-16: D8 3D DC 95
void unicode_string_view()
{
    using namespace std::literals::string_view_literals;

    // std::string_view := std::basic_string_view<char>
    // constexpr std::string_view love {"I❤💕C++"};
    constexpr auto love {"I❤💕C++"sv};
    static_assert(love.size() == 11);
    static_assert(love.length() == 11);
    static_assert(sizeof(std::string_view::value_type) == 1);

    // std::u8string_view := std::basic_string_view<char8_t>
    // constexpr std::u8string_view love8 {u8"I❤💕C++"};
    constexpr auto love8 {u8"I❤💕C++"sv};
    static_assert(love8.size() == 11);  // 11 char8_t's
    static_assert(love8.length() == 11);
    static_assert(sizeof(std::u8string_view::value_type) == 1);

    // std::basic_string_view<char16_t>
    constexpr std::u16string_view love16 {u"I❤💕C++"};
    static_assert(love16.size() == 7);  // 7 char16_t's
    static_assert(love16.length() == 7);
    static_assert(sizeof(std::u16string_view::value_type) == 2);

    // std::basic_string_view<char32_t>
    constexpr std::u32string_view love32 {U"I❤💕C++"};
    static_assert(love32.size() == 6);  // 6 char32_t's
    static_assert(love32.length() == 6);
    static_assert(sizeof(std::u32string_view::value_type) == 4);
    static_assert(sizeof(std::basic_string_view<char32_t>::value_type) == 4);
}

template<typename T>
void print_string_view_hex(const std::basic_string_view<T>& v)
{
    constexpr size_t digits = sizeof(T) * 2;   // 2 digits per byte

    std::cout
        << std::hex
        << std::setfill('0')
        << std::uppercase;

    // if the parameter is of type
    // `std::string_view` (i.e., `std::basic_string_view<char>`),
    // then for (auto c : love1) := for (char c : love),
    // which ends up printing like ffffffe2, not e2.
    // `for (unsigned char c : v)` works.
    for (auto c : v)
    {
        std::cout
            << std::setw(digits)
            << static_cast<unsigned int>(c)
            << ' ';
    }
    std::cout << std::endl;
}

int main()
{
    using namespace std::literals::string_view_literals;

    constexpr auto love {"I❤💕C++"sv};
    std::cout << love << std::endl;
    print_string_view_hex(love);

    constexpr auto love8 {u8"I❤💕C++"sv};
    // ostream does not provide operator<<(std::u8string_view)
    // std::cout << love8 << std::endl;
    print_string_view_hex(love8);

    constexpr auto love16 {u"I❤💕C++"sv};
    print_string_view_hex(love16);

    constexpr auto love32 {U"I❤💕C++"sv};
    print_string_view_hex(love32);
}