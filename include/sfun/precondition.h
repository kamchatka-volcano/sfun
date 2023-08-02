#ifndef SFUN_PRECONDITION_H
#define SFUN_PRECONDITION_H

#include "contract.h"
#include "utility.h"
#include <cstdint>
#include <functional>
#include <type_traits>

namespace sfun {

template<typename TCheck, typename T>
class arg_precondition {
public:
    template<typename... TArgs>
    constexpr arg_precondition(TArgs&&... val)
        : value_(std::forward<TArgs>(val)...)
    {
        std::invoke(TCheck{}, value_);
    }
    ~arg_precondition() = default;

    arg_precondition(const arg_precondition<TCheck, T>&) = delete;
    arg_precondition(arg_precondition<TCheck, T>&&) = delete;
    arg_precondition<TCheck, T>& operator=(const arg_precondition<TCheck, T>&) = delete;
    arg_precondition<TCheck, T>& operator=(arg_precondition<TCheck, T>&&) = delete;

    constexpr std::add_lvalue_reference_t<T> get()
    {
        return value_;
    }

    constexpr const std::add_lvalue_reference_t<T> get() const
    {
        return value_;
    }

    constexpr operator std::add_lvalue_reference_t<T>()
    {
        return value_;
    }

    constexpr operator const std::add_lvalue_reference_t<T>() const
    {
        return value_;
    }

private:
    T value_;
};

namespace detail {
struct check_not_empty {
    template<typename T>
    constexpr void operator()(T& x) const
    {
        sfun_precondition(!x.empty());
    }
};

struct check_valid {
    template<typename T>
    constexpr void operator()(T& x) const
    {
        sfun_precondition(x);
    }
};

enum class interval_type {
    closed,
    half_open,
    open
};

template<interval_type intervalType, typename T, T min, T max>
struct check_interval {
    static_assert(std::is_integral_v<T>);

    constexpr void operator()(T& x) const
    {
        if constexpr (intervalType == interval_type::closed)
            sfun_precondition(x >= min && x <= max);
        if constexpr (intervalType == interval_type::half_open)
            sfun_precondition(x >= min && x < max);
        if constexpr (intervalType == interval_type::open)
            sfun_precondition(x > min && x < max);
    }
};

struct check_not_negative {
    template<typename T>
    constexpr void operator()(T& x) const
    {
        sfun_precondition(x >= T{0});
    }
};

} //namespace detail

template<typename T>
using not_empty = arg_precondition<detail::check_not_empty, T>;

template<typename T>
using valid = arg_precondition<detail::check_valid, T>;

template<typename T, T min, T max>
using interval = arg_precondition<detail::check_interval<detail::interval_type::half_open, T, min, max>, T>;

template<typename T, T min, T max>
using interval_closed = arg_precondition<detail::check_interval<detail::interval_type::closed, T, min, max>, T>;

template<typename T, T min, T max>
using interval_open = arg_precondition<detail::check_interval<detail::interval_type::open, T, min, max>, T>;

template<typename T>
using not_negative = arg_precondition<detail::check_not_negative, T>;

} //namespace sfun

#endif //SFUN_PRECONDITION_H
