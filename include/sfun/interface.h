#ifndef SFUN_INTERFACE_H
#define SFUN_INTERFACE_H

#include <type_traits>

namespace sfun {

template<typename T>
class interface {
public:
    interface()
    {
        static_assert(
                std::is_base_of_v<interface<T>, T> && !std::is_convertible_v<T*, interface<T>*>,
                "T must be derived from interface<T> privately");
    }
    virtual ~interface() = default;
    interface(const interface&) = delete;
    interface& operator=(const interface&) = delete;
    interface(interface&&) = delete;
    interface& operator=(interface&&) = delete;
};

template<typename T>
struct access_token {
private:
    access_token(T&){};
    access_token(){};
    friend T;
};

namespace detail {

template<typename T>
struct AccessPermissionBase {
    void operator()(access_token<T>) {}
};

} //namespace detail

template<typename... Ts>
struct access_permission : private detail::AccessPermissionBase<Ts>... {
    template<typename TAccessToken>
    access_permission(TAccessToken accessToken)
    {
        operator()(accessToken);
    }
    using detail::AccessPermissionBase<Ts>::operator()...;
};

} //namespace sfun

#endif //SFUN_INTERFACE_H