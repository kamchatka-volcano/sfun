#ifndef SFUN_PATH_H
#define SFUN_PATH_H

#include "winconv.h"
#include <filesystem>
#include <string>
#include <string_view>

namespace sfun {

inline std::filesystem::path makePath(std::string_view str)
{
#ifdef _WIN32
    return toUtf16(str);
#else
    return str;
#endif
}

inline std::string pathString(const std::filesystem::path& path)
{
#ifdef _WIN32
    return toUtf8(path.wstring());
#else
    return path.string();
#endif
}

} //namespace sfun

#endif //SFUN_PATH_H
