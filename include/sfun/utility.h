#ifndef SFUN_UTILITY_H
#define SFUN_UTILITY_H

#include <cstddef>
#include <exception>
#include <type_traits>


namespace sfun{

[[noreturn]]
inline void unreachable() noexcept
{
    std::terminate();
}

template <typename T>
inline constexpr auto dependent_false = false;

template <class C>
constexpr auto ssize(const C& c)
    -> std::common_type_t<std::ptrdiff_t,
                          std::make_signed_t<decltype(c.size())>>
{
    using R = std::common_type_t<std::ptrdiff_t,
                                 std::make_signed_t<decltype(c.size())>>;
    return static_cast<R>(c.size());
}

template <class T, std::ptrdiff_t N>
constexpr std::ptrdiff_t ssize(const T (&)[N]) noexcept
{
    return N;
}

}

#endif //SFUN_UTILITY_H