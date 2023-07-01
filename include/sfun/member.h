#ifndef SFUN_MEMBER_H
#define SFUN_MEMBER_H

#include "type_traits.h"
#include <memory>
#include <type_traits>
#include <utility>

namespace sfun {

namespace detail {

template<typename T, typename V, std::enable_if_t<std::is_reference_v<T>>* = nullptr>
inline constexpr auto memberInit(V& val)
{
    return std::addressof(val);
}

template<typename T, typename V, std::enable_if_t<!std::is_reference_v<T>>* = nullptr>
inline constexpr decltype(auto) memberInit(V&& val)
{
    return std::forward<V>(val);
}

} //namespace detail

template<typename T>
struct member {
    static_assert(!std::is_array_v<T>, "sfun::member doesn't support arrays");

    template<
            typename TCheck = T,
            std::enable_if_t<
                    !std::is_reference_v<TCheck> && //
                    !std::is_const_v<TCheck> && //
                    !std::is_scalar_v<TCheck>>* = nullptr>
    constexpr member()
    {
    }

    template<
            typename V,
            typename TCheck = T,
            std::enable_if_t<!std::is_same_v<member<TCheck>, std::decay_t<V>>>* = nullptr>
    constexpr member(V&& val)
        : value_{detail::memberInit<T>(std::forward<V>(val))}
    {
    }

    constexpr const T& get() const
    {
        if constexpr (std::is_reference_v<T>)
            return *value_;
        else
            return value_;
    }

    constexpr T& get()
    {
        if constexpr (std::is_reference_v<T>)
            return *value_;
        else
            return value_;
    }

    constexpr operator const T&() const
    {
        if constexpr (std::is_reference_v<T>)
            return *value_;
        else
            return value_;
    }

    constexpr operator T&()
    {
        if constexpr (std::is_reference_v<T>)
            return *value_;
        else
            return value_;
    }

    template<typename V, std::enable_if_t<!std::is_same_v<V, member<T>>>* = nullptr>
    constexpr member<T>& operator=(V&& other)
    {
        get() = std::forward<V>(other);
        return *this;
    }

    template<typename CheckType = T, std::enable_if_t<std::is_constructible_v<bool, CheckType>>* = nullptr>
    constexpr explicit operator bool() const
    {
        return static_cast<bool>(get());
    }

    template<
            typename CheckType = T,
            std::enable_if_t<provides_member_access_v<CheckType> || std::is_pointer_v<CheckType>>* = nullptr>
    constexpr auto& operator->()
    {
        return value_;
    }

    template<
            typename CheckType = T,
            std::enable_if_t<provides_member_access_v<CheckType> || std::is_pointer_v<CheckType>>* = nullptr>
    constexpr const auto& operator->() const
    {
        return value_;
    }

    template<typename CheckType = T, std::enable_if_t<is_dereferencable_v<CheckType>>* = nullptr>
    constexpr auto& operator*()
    {
        return *value_;
    }

    template<typename CheckType = T, std::enable_if_t<is_dereferencable_v<CheckType>>* = nullptr>
    constexpr const auto& operator*() const
    {
        return *value_;
    }

    template<
            typename CheckType = T,
            std::enable_if_t<provides_array_element_access_v<CheckType> || std::is_pointer_v<CheckType>>* = nullptr>
    constexpr auto& operator[](std::size_t index)
    {
        return get()[index];
    }

    template<
            typename CheckType = T,
            std::enable_if_t<provides_array_element_access_v<CheckType> || std::is_pointer_v<CheckType>>* = nullptr>
    constexpr const auto& operator[](std::size_t index) const
    {
        return get()[index];
    }

    template<
            typename... TArgs,
            typename CheckType = T,
            std::enable_if_t<std::is_invocable_v<CheckType, TArgs...>>* = nullptr>
    constexpr decltype(auto) operator()(TArgs&&... args)
    {
        return get()(std::forward<TArgs>(args)...);
    }

    template<
            typename... TArgs,
            typename CheckType = T,
            std::enable_if_t<std::is_invocable_v<CheckType, TArgs...>>* = nullptr>
    constexpr decltype(auto) operator()(TArgs&&... args) const
    {
        return get()(std::forward<TArgs>(args)...);
    }

    template<typename V>
    constexpr bool operator==(const V& other) const
    {
        return get() == other;
    }

    template<typename V>
    constexpr bool operator!=(const V& other) const
    {
        return get() != other;
    }

    template<typename V>
    constexpr bool operator<=(const V& other) const
    {
        return get() <= other;
    }

    template<typename V>
    constexpr bool operator<(const V& other) const
    {
        return get() < other;
    }

    template<typename V>
    constexpr bool operator>=(const V& other) const
    {
        return get() >= other;
    }

    template<typename V>
    constexpr bool operator>(const V& other) const
    {
        return get() > other;
    }

private:
    std::conditional_t< //
            std::is_reference_v<T>,
            std::remove_reference_t<std::remove_const_t<T>>*,
            std::remove_const_t<T>>
            value_;
};

} //namespace sfun

#endif //SFUN_MEMBER_H