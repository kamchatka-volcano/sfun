#pragma once
#include <type_traits>
#include <optional>

namespace sfun::traits {

template<typename T, typename = void>
struct is_optional : std::false_type {};

template<typename T>
struct is_optional<std::optional<T>> : std::true_type {};

template <typename T>
inline constexpr auto is_optional_v = is_optional<T>::value;

template<typename T, typename = void>
struct remove_optional{
    using type = T;
};

template<typename T>
struct remove_optional<std::optional<T>>{
    using type = T;
};

template<typename T>
using remove_optional_t = typename remove_optional<T>::type;

template <typename, typename = void>
struct is_dynamic_sequence_container : std::false_type{};

template <typename T>
struct is_dynamic_sequence_container<T,
    std::void_t< typename T::value_type,
                 decltype(std::declval<T>().begin()),
                 decltype(std::declval<T>().end()),
                 decltype(std::declval<T>().emplace_back(std::declval<typename T::value_type>()))>
> : std::true_type{};

template <typename T>
inline constexpr auto is_dynamic_sequence_container_v = is_dynamic_sequence_container<T>::value;

template <typename, typename = void>
struct is_associative_container : std::false_type{};

template <typename T>
struct is_associative_container<T,
        std::void_t<typename T::key_type,
                    typename T::mapped_type,
                    decltype(std::declval<T>().begin()),
                    decltype(std::declval<T>().end()),
                    decltype(std::declval<T>().emplace(std::declval<typename T::key_type>(),
                                                       std::declval<typename T::mapped_type>()))>
> : std::true_type{};

template <typename T>
inline constexpr auto is_associative_container_v = is_associative_container<T>::value;

template <typename T>
inline constexpr auto dependent_false = false;

}