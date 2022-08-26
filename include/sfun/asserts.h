#pragma once
#include <exception>

namespace sfun::assert{

[[noreturn]]
inline void ensureNotReachable() noexcept
{
    std::terminate();
}

}