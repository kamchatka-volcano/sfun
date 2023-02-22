#ifndef SFUN_WINCONV_H
#define SFUN_WINCONV_H

#ifdef _WIN32
#include <windows.h>
#include <string>

namespace sfun {

inline std::string toUtf8(std::wstring_view utf16String)
{
    int count = WideCharToMultiByte(
            CP_UTF8,
            0,
            utf16String.data(),
            static_cast<int>(utf16String.size()),
            nullptr,
            0,
            nullptr,
            nullptr);
    auto utf8String = std::string(count, 0);
    WideCharToMultiByte(CP_UTF8, 0, utf16String.data(), -1, &utf8String[0], count, nullptr, nullptr);
    return utf8String;
}

inline std::wstring toUtf16(std::string_view utf8String)
{
    int count = MultiByteToWideChar(CP_UTF8, 0, utf8String.data(), static_cast<int>(utf8String.size()), nullptr, 0);
    auto utf16String = std::wstring(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, utf8String.data(), static_cast<int>(utf8String.size()), &utf16String[0], count);
    return utf16String;
}

} //namespace sfun

#endif

#endif //SFUN_WINCONV_H
