#ifndef SFUN_TYPE_LIST_H
#define SFUN_TYPE_LIST_H

#include "type_traits.h"
#include "utility.h"
#include <tuple>

namespace sfun {
template <typename... Ts>
class type_list;

namespace detail {

template <typename... T>
struct TypeList {};

template <typename TList, std::size_t I>
struct TypeListAt;

template <typename TFront, typename... TBack>
struct TypeListAt<TypeList<TFront, TBack...>, 0> {
    using type = TFront;
};

template <typename TFront, typename... TBack, std::size_t I>
struct TypeListAt<TypeList<TFront, TBack...>, I> {
    using type = typename TypeListAt<TypeList<TBack...>, I - 1>::type;
};

template <typename TList, std::size_t... I>
constexpr auto makeTypeListSlice(std::index_sequence<I...>) -> type_list<typename TypeListAt<TList, I>::type...>;

} // namespace detail

template <typename... Ts>
class type_list {
    using list = detail::TypeList<Ts...>;

public:
    static constexpr int size()
    {
        return sizeof...(Ts);
    };

    template <std::size_t I>
    static constexpr auto at()
    {
        return type_identity<typename detail::TypeListAt<list, I>::type>{};
    }

    template <std::size_t First, std::size_t Size>
    static auto slice()
    {
        static_assert(First + Size <= size());
        return decltype(detail::makeTypeListSlice<list>(make_index_range<First, First + Size>{})){};
    }
};

template <std::size_t I, typename... TListArgs>
constexpr auto get(const type_list<TListArgs...>&)
{
    return type_list<TListArgs...>::template at<I>();
}

template <typename... Args>
struct to_tuple<type_list<Args...>> {
    using type = std::tuple<Args...>;
};

template <typename T>
using to_tuple_t = typename to_tuple<T>::type;

} // namespace sfun

#endif // SFUN_TYPE_LIST_H
