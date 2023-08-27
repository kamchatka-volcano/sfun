#ifndef SFUN_OPTIONAL_REF_H
#define SFUN_OPTIONAL_REF_H
#include "contract.h"
#include "member.h"
#include "utility.h"
#include <exception>
#include <optional>

namespace sfun {

enum class const_propagation {
    disabled,
    enabled
};

template<typename T, const_propagation constPropagationMode = const_propagation::disabled>
class optional_ref {
    static_assert(!std::is_pointer_v<T>, "optional_ref type can't be a pointer");
    static_assert(!std::is_reference_v<T>, "optional_ref type can't be a reference");

public:
    constexpr optional_ref() = default;
    constexpr optional_ref(std::nullopt_t) noexcept {};

    template<
            typename TVal,
            typename TCheck = T,
            std::enable_if_t<!std::is_same_v<std::decay_t<TVal>, optional_ref<TCheck, constPropagationMode>>>* =
                    nullptr>
    constexpr optional_ref(TVal& val) noexcept
        : value_{std::addressof(val)}
    {
    }

    template<typename TVal>
    constexpr optional_ref(TVal* val) noexcept
        : value_{val}
    {
    }

    ~optional_ref() = default;
    constexpr optional_ref(const optional_ref& other) = default;
    constexpr optional_ref(optional_ref&&) = default;
    constexpr optional_ref& operator=(const optional_ref& other) = delete;
    constexpr optional_ref& operator=(optional_ref&&) = delete;

    template<typename TVal>
    constexpr void emplace(TVal& val) noexcept
    {
        value_ = std::addressof(val);
    }

    template<typename TVal>
    constexpr void emplace(TVal* val) noexcept
    {
        value_ = val;
    }

    constexpr void reset() noexcept
    {
        value_ = nullptr;
    }

    constexpr bool has_value() const noexcept
    {
        return value_ != nullptr;
    }

    template<
            const_propagation mode = constPropagationMode,
            std::enable_if_t<mode == const_propagation::disabled>* = nullptr>
    constexpr T& value() const
    {
        return deref(value_);
    }

    template<
            const_propagation mode = constPropagationMode,
            std::enable_if_t<mode == const_propagation::enabled>* = nullptr>
    constexpr const T& value() const
    {
        return deref(value_);
    }

    template<
            const_propagation mode = constPropagationMode,
            std::enable_if_t<mode == const_propagation::enabled>* = nullptr>
    constexpr T& value()
    {
        return deref(value_);
    }

    constexpr explicit operator bool() const noexcept
    {
        return has_value();
    }

    template<
            const_propagation mode = constPropagationMode,
            std::enable_if_t<mode == const_propagation::disabled>* = nullptr>
    constexpr T& operator*() const
    {
        return deref(value_);
    }

    template<
            const_propagation mode = constPropagationMode,
            std::enable_if_t<mode == const_propagation::enabled>* = nullptr>
    constexpr const T& operator*() const
    {
        return deref(value_);
    }

    template<
            const_propagation mode = constPropagationMode,
            std::enable_if_t<mode == const_propagation::enabled>* = nullptr>
    constexpr T& operator*()
    {
        return deref(value_);
    }

    template<
            const_propagation mode = constPropagationMode,
            std::enable_if_t<mode == const_propagation::disabled>* = nullptr>
    constexpr T* operator->() const
    {
        sfun_precondition(value_);
        return value_;
    }

    template<
            const_propagation mode = constPropagationMode,
            std::enable_if_t<mode == const_propagation::enabled>* = nullptr>
    constexpr const T* operator->() const
    {
        sfun_precondition(value_);
        return value_;
    }

    template<
            const_propagation mode = constPropagationMode,
            std::enable_if_t<mode == const_propagation::enabled>* = nullptr>
    constexpr T* operator->()
    {
        sfun_precondition(value_);
        return value_;
    }

    friend void swap(optional_ref<T, constPropagationMode>& lhs, optional_ref<T, constPropagationMode>& rhs) noexcept
    {
        std::swap(lhs.value_, rhs.value_);
    }

private:
    T* value_ = nullptr;
};

template<class T, class U, const_propagation constPropagationMode>
bool operator==(const optional_ref<T, constPropagationMode>& lhs, const optional_ref<U, constPropagationMode>& rhs)
{
    return static_cast<bool>(lhs) == static_cast<bool>(rhs) && (!lhs || *lhs == *rhs);
}

template<class T, class U, const_propagation constPropagationMode>
bool operator!=(const optional_ref<T, constPropagationMode>& lhs, const optional_ref<U, constPropagationMode>& rhs)
{
    return static_cast<bool>(lhs) != static_cast<bool>(rhs) || (static_cast<bool>(lhs) && *lhs != *rhs);
}

template<class T, class U, const_propagation constPropagationMode>
bool operator<(const optional_ref<T, constPropagationMode>& lhs, const optional_ref<U, constPropagationMode>& rhs)
{
    return static_cast<bool>(rhs) && (!lhs || *lhs < *rhs);
}

template<class T, class U, const_propagation constPropagationMode>
bool operator<=(const optional_ref<T, constPropagationMode>& lhs, const optional_ref<U, constPropagationMode>& rhs)
{
    return !lhs || (static_cast<bool>(rhs) && *lhs <= *rhs);
}

template<class T, class U, const_propagation constPropagationMode>
bool operator>(const optional_ref<T, constPropagationMode>& lhs, const optional_ref<U, constPropagationMode>& rhs)
{
    return static_cast<bool>(lhs) && (!rhs || *lhs > *rhs);
}
template<class T, class U, const_propagation constPropagationMode>
bool operator>=(const optional_ref<T, constPropagationMode>& lhs, const optional_ref<U, constPropagationMode>& rhs)
{
    return !rhs || (static_cast<bool>(lhs) && *lhs >= *rhs);
}

template<class T, const_propagation constPropagationMode>
bool operator==(const optional_ref<T, constPropagationMode>& lhs, std::nullopt_t)
{
    return !lhs;
}

template<class T, const_propagation constPropagationMode>
bool operator==(std::nullopt_t, const optional_ref<T, constPropagationMode>& rhs)
{
    return !rhs;
}

template<class T, const_propagation constPropagationMode>
bool operator!=(const optional_ref<T, constPropagationMode>& lhs, std::nullopt_t)
{
    return static_cast<bool>(lhs);
}

template<class T, const_propagation constPropagationMode>
bool operator!=(std::nullopt_t, const optional_ref<T, constPropagationMode>& rhs)
{
    return static_cast<bool>(rhs);
}

template<class T, const_propagation constPropagationMode>
bool operator<(const optional_ref<T, constPropagationMode>&, std::nullopt_t)
{
    return false;
}

template<class T, const_propagation constPropagationMode>
bool operator<(std::nullopt_t, const optional_ref<T, constPropagationMode>& rhs)
{
    return static_cast<bool>(rhs);
}

template<class T, const_propagation constPropagationMode>
bool operator<=(const optional_ref<T, constPropagationMode>& lhs, std::nullopt_t)
{
    return !lhs;
}

template<class T, const_propagation constPropagationMode>
bool operator<=(std::nullopt_t, const optional_ref<T, constPropagationMode>&)
{
    return true;
}

template<class T, const_propagation constPropagationMode>
bool operator>(const optional_ref<T, constPropagationMode>& lhs, std::nullopt_t)
{
    return static_cast<bool>(lhs);
}

template<class T, const_propagation constPropagationMode>
bool operator>(std::nullopt_t, const optional_ref<T, constPropagationMode>&)
{
    return false;
}

template<class T, const_propagation constPropagationMode>
bool operator>=(const optional_ref<T, constPropagationMode>&, std::nullopt_t)
{
    return true;
}

template<class T, const_propagation constPropagationMode>
bool operator>=(std::nullopt_t, const optional_ref<T, constPropagationMode>& rhs)
{
    return !rhs;
}

template<class T, class U, const_propagation constPropagationMode>
bool operator==(const optional_ref<T, constPropagationMode>& lhs, const U& rhs)
{
    return lhs && *lhs == rhs;
}

template<class T, class U, const_propagation constPropagationMode>
bool operator==(const U& lhs, const optional_ref<T, constPropagationMode>& rhs)
{
    return rhs && lhs == *rhs;
}

template<class T, class U, const_propagation constPropagationMode>
bool operator!=(const optional_ref<T, constPropagationMode>& lhs, const U& rhs)
{
    return !lhs || *lhs != rhs;
}

template<class T, class U, const_propagation constPropagationMode>
bool operator!=(const U& lhs, const optional_ref<T, constPropagationMode>& rhs)
{
    return !rhs || lhs != *rhs;
}

template<class T, class U, const_propagation constPropagationMode>
bool operator<(const optional_ref<T, constPropagationMode>& lhs, const U& rhs)
{
    return !lhs || *lhs < rhs;
}

template<class T, class U, const_propagation constPropagationMode>
bool operator<(const U& lhs, const optional_ref<T, constPropagationMode>& rhs)
{
    return rhs && lhs < *rhs;
}

template<class T, class U, const_propagation constPropagationMode>
bool operator<=(const optional_ref<T, constPropagationMode>& lhs, const U& rhs)
{
    return !lhs || *lhs <= rhs;
}

template<class T, class U, const_propagation constPropagationMode>
bool operator<=(const U& lhs, const optional_ref<T, constPropagationMode>& rhs)
{
    return rhs && lhs <= *rhs;
}

template<class T, class U, const_propagation constPropagationMode>
bool operator>(const optional_ref<T, constPropagationMode>& lhs, const U& rhs)
{
    return lhs && *lhs > rhs;
}

template<class T, class U, const_propagation constPropagationMode>
bool operator>(const U& lhs, const optional_ref<T, constPropagationMode>& rhs)
{
    return !rhs || lhs > *rhs;
}

template<class T, class U, const_propagation constPropagationMode>
bool operator>=(const optional_ref<T, constPropagationMode>& lhs, const U& rhs)
{
    return lhs && *lhs >= rhs;
}

template<class T, class U, const_propagation constPropagationMode>
bool operator>=(const U& lhs, const optional_ref<T, constPropagationMode>& rhs)
{
    return !rhs || lhs >= *rhs;
}

template<typename T, const_propagation constPropagationMode = const_propagation::disabled>
class optional_ref_wrapper {
    using optional_ref_t = optional_ref<T, constPropagationMode>;
    static_assert(!std::is_pointer_v<T>, "optional_ref_wrapper type can't be a pointer");
    static_assert(!std::is_reference_v<T>, "optional_ref_wrapper type can't be a reference");

public:
    optional_ref_wrapper() = default;
    optional_ref_wrapper(std::nullopt_t) noexcept {};
    optional_ref_wrapper(optional_ref_t ref) noexcept
        : ref_{std::move(ref)}
    {
    }
    ~optional_ref_wrapper() = default;
    optional_ref_wrapper(const optional_ref_wrapper& other) = default;
    optional_ref_wrapper(optional_ref_wrapper&&) = default;
    optional_ref_wrapper& operator=(const optional_ref_wrapper& other)
    {
        auto tmp = optional_ref_wrapper{other};
        std::swap(*this, tmp);
        return *this;
    }
    optional_ref_wrapper& operator=(optional_ref_wrapper&& other) noexcept
    {
        swap(ref_, other.ref_);
        return *this;
    }

    const optional_ref_t& get() const noexcept
    {
        return ref_;
    }

    optional_ref_t& get() noexcept
    {
        return ref_;
    }

    operator optional_ref_t&() noexcept
    {
        return ref_;
    }

    operator const optional_ref_t&() const noexcept
    {
        return ref_;
    }

private:
    optional_ref_t ref_;
};

template<typename T, const_propagation constPropagationMode>
struct member<optional_ref<T, constPropagationMode>> {
    using optional_ref_t = optional_ref<T, constPropagationMode>;
    constexpr member() = default;

    template<typename V>
    constexpr member(optional_ref<V> val) noexcept
        : value_{val}
    {
    }

    template<
            typename V,
            typename TCheck = T,
            std::enable_if_t<!std::is_same_v<member<TCheck>, std::decay_t<V>>>* = nullptr>
    constexpr member(V& val) noexcept
        : value_{optional_ref_t{val}}
    {
    }

    constexpr optional_ref_t& get() noexcept
    {
        return value_;
    }

    constexpr const optional_ref_t& get() const noexcept
    {
        return value_;
    }

    constexpr operator optional_ref_t&() noexcept
    {
        return value_;
    }

    constexpr operator const optional_ref_t&() const noexcept
    {
        return value_;
    }

    constexpr explicit operator bool() const noexcept
    {
        return static_cast<bool>(get());
    }

    constexpr auto& operator*()
    {
        return *get();
    }

    constexpr auto& operator*() const
    {
        return *get();
    }

    template<
            const_propagation mode = constPropagationMode,
            std::enable_if_t<mode == const_propagation::disabled>* = nullptr>
    constexpr T* operator->() const
    {
        return &get().value();
    }
    template<
            const_propagation mode = constPropagationMode,
            std::enable_if_t<mode == const_propagation::enabled>* = nullptr>
    constexpr T* operator->()
    {
        return &get().value();
    }
    template<
            const_propagation mode = constPropagationMode,
            std::enable_if_t<mode == const_propagation::enabled>* = nullptr>
    constexpr const T* operator->() const
    {
        return &get().value();
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
    optional_ref_wrapper<T, constPropagationMode> value_;
};

template<typename T>
using optional_cpref = optional_ref<T, const_propagation::enabled>;

template<typename T>
using optional_cpref_wrapper = optional_ref_wrapper<T, const_propagation::enabled>;

} //namespace sfun

#endif //SFUN_OPTIONAL_REF_H
