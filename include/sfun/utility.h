#ifndef SFUN_UTILITY_H
#define SFUN_UTILITY_H

#include <cstddef>
#include <exception>
#include <type_traits>
#include <utility>

namespace sfun {

[[noreturn]] inline void unreachable() noexcept
{
    std::terminate();
}

template<typename T>
inline constexpr auto dependent_false = false;

using ssize_t = std::ptrdiff_t;
using index_t = std::ptrdiff_t;

template<class C>
constexpr auto ssize(const C& c) -> std::common_type_t<ssize_t, std::make_signed_t<decltype(c.size())>>
{
    using R = std::common_type_t<ssize_t, std::make_signed_t<decltype(c.size())>>;
    return static_cast<R>(c.size());
}

template<class T, std::ptrdiff_t N>
constexpr ssize_t ssize(const T (&)[N]) noexcept
{
    return N;
}

namespace detail {
template<std::size_t N, std::size_t... Ints>
constexpr auto shiftSequence(std::index_sequence<Ints...>) -> std::index_sequence<N + Ints...>;
}

template<std::size_t Begin, std::size_t End>
using make_index_range = decltype(detail::shiftSequence<Begin>(std::make_index_sequence<End - Begin>()));

} //namespace sfun

#endif //SFUN_UTILITY_H