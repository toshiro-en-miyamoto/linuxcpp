#include <cstddef>
#include <span>
#include <algorithm>

template <typename T, std::size_t N, std::size_t M> [[nodiscard]]
constexpr bool contains(std::span<T,N> span, std::span<T,M> sub)
{
    return std::search(span.begin(), span.end(), sub.begin(), sub.end()) != span.end();
}

int main()
{
    constexpr int seq[] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    static_assert(
        contains(std::span{seq}, std::span{seq, 3})
     && !contains(std::span{seq, 3}, std::span{seq})
    );
}
