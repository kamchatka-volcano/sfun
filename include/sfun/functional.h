#ifndef SFUN_FUNCTIONAL_H
#define SFUN_FUNCTIONAL_H

#include "type_list.h"
#include <functional>
#include <utility>

namespace sfun {

template <typename T>
struct callable_signature;

template <typename R, typename... Args>
struct callable_signature<std::function<R(Args...)>> {
    using return_type = R;
    using args = type_list<Args...>;
};

template <typename TCallable>
using callable_args = typename callable_signature<decltype(std::function{std::declval<TCallable>()})>::args;

template <typename TCallable>
using callable_return_type =
        typename callable_signature<decltype(std::function{std::declval<TCallable>()})>::return_type;

template <class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template <class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

} // namespace sfun

#endif // SFUN_FUNCTIONAL_H
