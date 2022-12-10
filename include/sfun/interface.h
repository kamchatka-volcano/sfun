#pragma once
#include <type_traits>

namespace sfun{

template<typename T>
class Interface{
public:
    Interface()
    {
        static_assert(std::is_base_of_v<Interface<T>, T> && !std::is_convertible_v<T*, Interface<T>*>,
                "T must be derived from Interface<T> privately");
    }
    virtual ~Interface() = default;
    Interface(const Interface&) = delete;
    Interface& operator=(const Interface&) = delete;
    Interface(Interface&&) = delete;
    Interface& operator=(Interface&&) = delete;
};

}