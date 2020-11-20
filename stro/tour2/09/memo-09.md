# Strings and Regular Expressions

# Strings

A `string` is mutable.

```c++
string name = "Niels Stroustrup";
string s = name.substr(6,10);  // a copy of substring
name.replace(0,5,"nicholas");  // a string is mutable
name[0] = toupper(name[0]);    // toupper() does not modify it
```

A string literal is by definition a `const char*`. To get a literal of type `std::string` use a s suffix.

```c++
using namespace std::literals::string_literals;
auto s = "Cat"s;   // a std::string
auto p = "Dog";    // a C-style string: a const char*
```

To handle multiple character sets, `string` is really an alias for a general template `basic_string` with the character type char:

```c++
template<typename Char>
class basic_string {
    // ... string of Char ...
};

using string = basic_string<char>
```

## String Literals (C++20)

Excerpts from `https://en.cppreference.com/w/cpp/language/string_literal`.

```c++
(1) "..."
(2) L"..."
(3) u8"..."
(4) u"..."
(5) U"..."
```

1) *Narrow multibyte string* literal. The type of an unprefixed string literal is `const char[N]`, where `N` is the size of the string in code units of the execution narrow encoding, including the null terminator.
2) *Wide string* literal. The type of a `L"..."` string literal is `const wchar_t[N]`, where `N` is the size of the string in code units of the execution wide encoding, including the null terminator.
3) *UTF-8 encoded string* literal. The type of a `u8"..."` string literal is `const char[N]` (until C++20) `const char8_t[N]` (since C++20), where `N` is the size of the string in UTF-8 code units including the null terminator.
4) *UTF-16 encoded string* literal. The type of a `u"..."` string literal is `const char16_t[N]`, where `N` is the size of the string in UTF-16 code units including the null terminator.
5) *UTF-32 encoded string* literal. The type of a `U"..."` string literal is `const char32_t[N]`, where `N` is the size of the string in UTF-32 code units including the null terminator.

## String Views

A string_view is like a pointer or a reference in that it does not own the characters it points to.

```c++
#include <string_view>
#include <iostream>
#include <iomanip>

template<typename T>
void print_string_view_hex(const std::basic_string_view<T>& v)
{
    constexpr size_t digits = sizeof(T) * 2;
    std::cout << std::hex
        << std::setfill('0') 
        << std::uppercase;
    for (auto c : v)
    {
        std::cout << std::setw(digits)
            << static_cast<unsigned int>(c) << ' ';
    }
    std::cout << std::endl;
}
```

We can call `print_string_view_hex()`:

```c++
int main()
{
    using namespace std::literals::string_view_literals;

    // constexpr std::string_view love {"I❤💕C++"};
    constexpr auto love {"I❤💕C++"sv};
    print_string_view_hex(love);

    // constexpr std::u8string_view love8 {u8"I❤💕C++"};
    constexpr auto love8 {u8"I❤💕C++"sv};
    print_string_view_hex(love8);

    // constexpr std::u16string_view love16 {u"I❤💕C++"};
    constexpr auto love16 {u"I❤💕C++"sv};
    print_string_view_hex(love16);

    // constexpr std::u32string_view love32 {U"I❤💕C++"};
    constexpr auto love32 {U"I❤💕C++"sv};
    print_string_view_hex(love32);
}
```

Note the use of the `sv` (“string view”) suffix.
Why bother with a suffix? The reason is that when we pass `"..."` we need to construct a `string_view` from a `const char*` and that requires counting the characters. For `"..."sv` the length is computed at compile time.

```c++
49 FFFFFFE2 FFFFFF9D FFFFFFA4 FFFFFFF0 FFFFFF9F FFFFFF92 FFFFFF95 43 2B 2B 
49 E2 9D A4 F0 9F 92 95 43 2B 2B 
0049 2764 D83D DC95 0043 002B 002B 
00000049 00002764 0001F495 00000043 0000002B 0000002B
```

## Regular Expressions

```c++
// U.S. postal code pattern: XXddddd-dddd and variants
regex pat {R"(\w{2}\s*\d{5}(−\d{4})?)"};
```

To express the pattern, I use a raw string literal starting with `R"(...)"`. This allows backslashes and quotes to be used directly in the string.

We can define a `regex_iterator` for iterating over a sequence of characters finding matches for a pattern.

```c++
void test()
{
    string input = "aa as; asd ++e^asdf asdfg";
    regex pat {R"(\s+(\w+))"};
    for (sregex_iterator p(input.begin(), input.end(), pat);
        p!=sregex_iterator{};
        ++p)
    {
        cout << (*p)[1] << '\n';
    }
}
// this outputs
// as
// asd
// asdfg
```

We missed the first word, aa, because it has no preceding whitespace.

The regular expression notation can be adjusted to match various standards. The default regular expression notation is that of ECMAScript.
