#ifndef SFUN_WSTRINGCONV_H
#define SFUN_WSTRINGCONV_H

#ifdef _WIN32
#include "detail/external/utf8.h"
#endif

#include "utility.h"
#include <string>

namespace sfun {

template<typename T = void>
inline std::string from_wstring([[maybe_unused]] std::wstring_view utf16String)
{
#ifdef _WIN32
    auto result = std::string{};
    utf8::utf16to8(utf16String.begin(), utf16String.end(), std::back_inserter(result));
    return result;
#else
    static_assert(
            dependent_false<T>,
            "In order to avoid misuse, the fromWString() function can only be used on the Windows platform.");
    return {};
#endif
}

template<typename T = void>
inline std::wstring to_wstring([[maybe_unused]] std::string_view utf8String)
{
#ifdef _WIN32
    auto result = std::wstring{};
    utf8::utf8to16(utf8String.begin(), utf8String.end(), std::back_inserter(result));
    return result;
#else
    static_assert(
            dependent_false<T>,
            "In order to avoid misuse, the toWString() function can only be used on the Windows platform.");
    return {};
#endif
}

} //namespace sfun

#endif //SFUN_WSTRINGCONV_H
