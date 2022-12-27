#ifndef SFUN_TYPE_LIST_H
#define SFUN_TYPE_LIST_H

#include "type_traits.h"
#include <tuple>

namespace sfun::detail {

template <typename... T>
struct TypeList {};

template <typename TTypeList, int I>
struct TypeListAt;

template <typename TFront, typename... TBack>
struct TypeListAt<TypeList<TFront, TBack...>, 0> {
    using type = TFront;
};

template <typename TFront, typename... TBack, int I>
struct TypeListAt<TypeList<TFront, TBack...>, I> {
    using type = typename TypeListAt<TypeList<TBack...>, I - 1>::type;
};

} // namespace sfun::detail

namespace sfun {

template <typename... Ts>
class type_list {
    using list = detail::TypeList<Ts...>;

public:
    using tuple = std::tuple<Ts...>;

    static constexpr int size()
    {
        return sizeof...(Ts);
    };

    template <int I>
    static constexpr auto at()
    {
        return type_identity<typename detail::TypeListAt<list, I>::type>{};
    }
};

} // namespace sfun

#endif // SFUN_TYPE_LIST_H
