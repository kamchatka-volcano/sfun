#ifndef SFUN_FUNCTIONAL_H
#define SFUN_FUNCTIONAL_H

#include "type_list.h"
#include <functional>
#include <utility>

namespace sfun {

namespace detail {

template<typename>
struct function_guide_helper {};

template<typename Res, typename Tp, typename... Args>
struct function_guide_helper<Res (Tp::*)(Args...)> {
    using type = Res(Args...);
};

template<typename R, typename Tp, typename... Args>
struct function_guide_helper<R (Tp::*)(Args...)&> {
    using type = R(Args...);
};

template<typename R, typename Tp, typename... Args>
struct function_guide_helper<R (Tp::*)(Args...) const> {
    using type = R(Args...);
};

template<typename R, typename Tp, typename... Args>
struct function_guide_helper<R (Tp::*)(Args...) const&> {
    using type = R(Args...);
};

template<typename Res, typename Tp, typename... Args>
struct function_guide_helper<Res (Tp::*)(Args...) noexcept> {
    using type = Res(Args...);
};

template<typename R, typename Tp, typename... Args>
struct function_guide_helper<R (Tp::*)(Args...) & noexcept> {
    using type = R(Args...);
};

template<typename R, typename Tp, typename... Args>
struct function_guide_helper<R (Tp::*)(Args...) const noexcept> {
    using type = R(Args...);
};

template<typename R, typename Tp, typename... Args>
struct function_guide_helper<R (Tp::*)(Args...) const & noexcept> {
    using type = R(Args...);
};

template<typename Signature>
struct callable_storage;

template<typename R, typename... Args>
struct callable_storage<R(Args...)> {
    template<typename TFunctor>
    callable_storage(TFunctor&&)
    {
    }
};

template<class R, class... ArgTypes>
callable_storage(R (*)(ArgTypes...)) -> callable_storage<R(ArgTypes...)>;

template<typename Functor, typename Signature = typename function_guide_helper<decltype(&Functor::operator())>::type>
callable_storage(Functor&&) -> callable_storage<Signature>;

template<typename Functor, typename Signature = typename function_guide_helper<decltype(&Functor::operator())>::type>
callable_storage(Functor&) -> callable_storage<Signature>;

} //namespace detail

template<typename T>
struct callable_signature;

template<typename R, typename... Args>
struct callable_signature<detail::callable_storage<R(Args...)>> {
    using return_type = R;
    using args = type_list<Args...>;
};

template<typename TCallable>
using callable_args = typename callable_signature<decltype(detail::callable_storage{std::declval<TCallable>()})>::args;

template<typename TCallable>
using callable_return_type =
        typename callable_signature<decltype(detail::callable_storage{std::declval<TCallable>()})>::return_type;

template<class... Ts>
struct overloaded : Ts... {
    using Ts::operator()...;
};
template<class... Ts>
overloaded(Ts...) -> overloaded<Ts...>;

template<typename F, typename... Args>
auto try_invoke(F&& f, Args&&... args) noexcept
{
    if constexpr (std::is_same_v<std::invoke_result_t<F, Args...>, void>) {
        try {
            std::invoke(std::forward<F>(f), std::forward<Args>(args)...);
            return true;
        }
        catch (...) {
            return false;
        }
    }
    else {
        try {
            return std::optional{std::invoke(std::forward<F>(f), std::forward<Args>(args)...)};
        }
        catch (...) {
            return std::optional<std::invoke_result_t<F, Args...>>{std::nullopt};
        }
    }
}

} //namespace sfun

#endif //SFUN_FUNCTIONAL_H
