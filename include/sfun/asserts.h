#ifndef SFUN_ASSERTS_H
#define SFUN_ASSERTS_H

#include <exception>
#include "assert_macros.h"

namespace sfun::assert{

[[noreturn]]
inline void ensureNotReachable() noexcept
{
    std::terminate();
}

}

#endif //SFUN_ASSERTS_H