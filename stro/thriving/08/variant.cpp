#include <variant>
#include <string>
#include <array>
#include <iostream>
#include <type_traits>

using var_t = std::variant<int, long, double, std::string>;

// helper type for the visitor
template<typename... Ts> struct overloaded : Ts... { using Ts::operator()...; };
// explicit deduction guide
template<typename... Ts> overloaded(Ts...) -> overloaded<Ts...>;

int main()
{
    using namespace std::literals;
    std::array<var_t, 4> vars{1, 2L, 3.4, "hello"s};

    for (auto& var : vars) { // void visitor
        std::visit([](auto&& arg){ std::cout << arg << '\n'; }, var);
    }
    for (auto& var : vars) {
        auto v = std::visit([](auto&& arg) -> var_t { return arg + arg; }, var);
        std::visit([](auto&& arg){ std::cout << arg << '\n'; }, v);
    }
    for (auto& var : vars) {
        std::visit([](auto&& arg){
            using T = std::decay_t<decltype(arg)>;
            if constexpr (std::is_same_v<T, int>)
                std::cout << "int: " << arg << '\n';
            else if constexpr (std::is_same_v<T, long>)
                std::cout << "long: " << arg << '\n';
            else if constexpr (std::is_same_v<T, double>)
                std::cout << "double: " << arg << '\n';
            else if constexpr (std::is_same_v<T, std::string>)
                std::cout << "string: " << arg << '\n';
        }, var);
    }
    for (auto& var : vars) {
        std::visit(overloaded{
            [](auto arg){ std::cout << arg << '\n'; },
            [](double arg){ std::cout << "double: " << arg << '\n'; },
            [](const std::string& arg) { std::cout << "string: " << arg << '\n'; }
        }, var);
    }
}
