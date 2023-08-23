#ifndef SFUN_OPTIONAL_REF_H
#define SFUN_OPTIONAL_REF_H
#include "contract.h"
#include "member.h"
#include "utility.h"
#include <exception>
#include <optional>

namespace sfun {

template<typename T>
class optional_ref {
    static_assert(!std::is_pointer_v<T>, "optional_ref type can't be a pointer");
    static_assert(!std::is_reference_v<T>, "optional_ref type can't be a reference");

public:
    constexpr optional_ref() = default;
    constexpr optional_ref(std::nullopt_t) noexcept {};

    template<
            typename TVal,
            typename TCheck = T,
            std::enable_if_t<!std::is_same_v<std::decay_t<TVal>, optional_ref<TCheck>>>* = nullptr>
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

    constexpr const T& value() const
    {
        return deref(value_);
    }

    constexpr T& value()
    {
        return deref(value_);
    }

    constexpr explicit operator bool() const noexcept
    {
        return has_value();
    }

    constexpr const T& operator*() const
    {
        return deref(value_);
    }

    constexpr T& operator*()
    {
        return deref(value_);
    }

    constexpr const T* operator->() const
    {
        sfun_precondition(value_);
        return value_;
    }

    constexpr T* operator->()
    {
        sfun_precondition(value_);
        return value_;
    }

    friend void swap(optional_ref<T>& lhs, optional_ref<T>& rhs) noexcept
    {
        std::swap(lhs.value_, rhs.value_);
    }

private:
    T* value_ = nullptr;
};

template<class T, class U>
bool operator==(const optional_ref<T>& lhs, const optional_ref<U>& rhs)
{
    return static_cast<bool>(lhs) == static_cast<bool>(rhs) && (!lhs || *lhs == *rhs);
}

template<class T, class U>
bool operator!=(const optional_ref<T>& lhs, const optional_ref<U>& rhs)
{
    return static_cast<bool>(lhs) != static_cast<bool>(rhs) || (static_cast<bool>(lhs) && *lhs != *rhs);
}

template<class T, class U>
bool operator<(const optional_ref<T>& lhs, const optional_ref<U>& rhs)
{
    return static_cast<bool>(rhs) && (!lhs || *lhs < *rhs);
}

template<class T, class U>
bool operator<=(const optional_ref<T>& lhs, const optional_ref<U>& rhs)
{
    return !lhs || (static_cast<bool>(rhs) && *lhs <= *rhs);
}

template<class T, class U>
bool operator>(const optional_ref<T>& lhs, const optional_ref<U>& rhs)
{
    return static_cast<bool>(lhs) && (!rhs || *lhs > *rhs);
}
template<class T, class U>
bool operator>=(const optional_ref<T>& lhs, const optional_ref<U>& rhs)
{
    return !rhs || (static_cast<bool>(lhs) && *lhs >= *rhs);
}

template<class T>
bool operator==(const optional_ref<T>& lhs, std::nullopt_t)
{
    return !lhs;
}

template<class T>
bool operator==(std::nullopt_t, const optional_ref<T>& rhs)
{
    return !rhs;
}

template<class T>
bool operator!=(const optional_ref<T>& lhs, std::nullopt_t)
{
    return static_cast<bool>(lhs);
}

template<class T>
bool operator!=(std::nullopt_t, const optional_ref<T>& rhs)
{
    return static_cast<bool>(rhs);
}

template<class T>
bool operator<(const optional_ref<T>&, std::nullopt_t)
{
    return false;
}

template<class T>
bool operator<(std::nullopt_t, const optional_ref<T>& rhs)
{
    return static_cast<bool>(rhs);
}

template<class T>
bool operator<=(const optional_ref<T>& lhs, std::nullopt_t)
{
    return !lhs;
}

template<class T>
bool operator<=(std::nullopt_t, const optional_ref<T>&)
{
    return true;
}

template<class T>
bool operator>(const optional_ref<T>& lhs, std::nullopt_t)
{
    return static_cast<bool>(lhs);
}

template<class T>
bool operator>(std::nullopt_t, const optional_ref<T>&)
{
    return false;
}

template<class T>
bool operator>=(const optional_ref<T>&, std::nullopt_t)
{
    return true;
}

template<class T>
bool operator>=(std::nullopt_t, const optional_ref<T>& rhs)
{
    return !rhs;
}

template<class T, class U>
bool operator==(const optional_ref<T>& lhs, const U& rhs)
{
    return lhs && *lhs == rhs;
}

template<class T, class U>
bool operator==(const U& lhs, const optional_ref<T>& rhs)
{
    return rhs && lhs == *rhs;
}

template<class T, class U>
bool operator!=(const optional_ref<T>& lhs, const U& rhs)
{
    return !lhs || *lhs != rhs;
}

template<class T, class U>
bool operator!=(const U& lhs, const optional_ref<T>& rhs)
{
    return !rhs || lhs != *rhs;
}

template<class T, class U>
bool operator<(const optional_ref<T>& lhs, const U& rhs)
{
    return !lhs || *lhs < rhs;
}

template<class T, class U>
bool operator<(const U& lhs, const optional_ref<T>& rhs)
{
    return rhs && lhs < *rhs;
}

template<class T, class U>
bool operator<=(const optional_ref<T>& lhs, const U& rhs)
{
    return !lhs || *lhs <= rhs;
}

template<class T, class U>
bool operator<=(const U& lhs, const optional_ref<T>& rhs)
{
    return rhs && lhs <= *rhs;
}

template<class T, class U>
bool operator>(const optional_ref<T>& lhs, const U& rhs)
{
    return lhs && *lhs > rhs;
}

template<class T, class U>
bool operator>(const U& lhs, const optional_ref<T>& rhs)
{
    return !rhs || lhs > *rhs;
}

template<class T, class U>
bool operator>=(const optional_ref<T>& lhs, const U& rhs)
{
    return lhs && *lhs >= rhs;
}

template<class T, class U>
bool operator>=(const U& lhs, const optional_ref<T>& rhs)
{
    return !rhs || lhs >= *rhs;
}

template<typename T>
class optional_ref_wrapper {
    static_assert(!std::is_pointer_v<T>, "optional_ref_wrapper type can't be a pointer");
    static_assert(!std::is_reference_v<T>, "optional_ref_wrapper type can't be a reference");

public:
    optional_ref_wrapper() = default;
    optional_ref_wrapper(std::nullopt_t) noexcept {};
    optional_ref_wrapper(optional_ref<T> ref) noexcept
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

    const optional_ref<T>& get() const noexcept
    {
        return ref_;
    }

    optional_ref<T>& get() noexcept
    {
        return ref_;
    }

    operator optional_ref<T>&() noexcept
    {
        return ref_;
    }

    operator const optional_ref<T>&() const noexcept
    {
        return ref_;
    }

private:
    optional_ref<T> ref_;
};

template<typename T>
struct member<optional_ref<T>> {
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
        : value_{optional_ref<T>{val}}
    {
    }

    constexpr optional_ref<T>& get() noexcept
    {
        return value_;
    }

    constexpr const optional_ref<T>& get() const noexcept
    {
        return value_;
    }

    constexpr operator optional_ref<T>&() noexcept
    {
        return value_;
    }

    constexpr operator const optional_ref<T>&() const noexcept
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

    constexpr const auto& operator*() const
    {
        return *get();
    }

    constexpr T* operator->()
    {
        return &get().value();
    }

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
    optional_ref_wrapper<T> value_;
};

} //namespace sfun

#endif //SFUN_OPTIONAL_REF_H
