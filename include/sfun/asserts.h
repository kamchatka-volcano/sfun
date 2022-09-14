#ifndef SFUN_ASSERTS_H
#define SFUN_ASSERTS_H

#include <exception>

namespace sfun::assert{

[[noreturn]]
inline void ensureNotReachable() noexcept
{
    std::terminate();
}

}

#endif //SFUN_ASSERTS_H