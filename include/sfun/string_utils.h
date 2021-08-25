#pragma once
#include <string>
#include <vector>
#include <algorithm>

namespace sfun{
namespace string_utils{

template <typename TStr>
std::string trimFront(TStr&& str)
{
    auto res = std::string{std::forward<TStr>(str)};
    res.erase(res.begin(), std::find_if(res.begin(), res.end(), [](int ch) {
        return !std::isspace(ch);
    }));
    return res;
}

template <typename TStr>
std::string trimBack(TStr&& str)
{
    auto res = std::string{std::forward<TStr>(str)};
    res.erase(std::find_if(res.rbegin(), res.rend(), [](int ch) {
        return !std::isspace(ch);
    }).base(), res.end());
    return res;
}

template <typename TStr>
std::string trim(TStr&& str)
{
    return trimBack(trimFront(std::forward<TStr>(str)));
}

inline std::vector<std::string> split(const std::string& str, const std::string& delim = " ", bool trimmed = true)
{
    if (delim.empty() || str.empty())
        return std::vector<std::string>{str};

    auto result = std::vector<std::string>{};
    auto delimPos = std::size_t{0};
    auto pos = std::size_t{0};
    while (pos < str.size()){
        delimPos = str.find(delim, delimPos);
        if (delimPos == std::string::npos)
            delimPos = str.size();
        auto part = str.substr(pos, delimPos - pos);
        if (trimmed)
            part = trim(part);
        if (!part.empty())
            result.push_back(part);
        delimPos += delim.size();
        pos = delimPos;
    }
    return result;
}

template <typename TStr>
std::string replace(TStr&& str, const std::string& subStr, const std::string& val)
{
    auto res = std::string{std::forward<TStr>(str)};
    if (subStr.empty())
        return res;

    auto pos = res.find(subStr);
    while (pos != std::string::npos){
        res.replace(pos, subStr.size(), val);
        pos = res.find(subStr, pos + val.size());
    }
    return res;
}

inline bool startsWith(const std::string& str, const std::string& val)
{
    auto res = str.find(val);
    return res == 0;
}

inline bool endsWith(const std::string& str, const std::string& val)
{
    if (val.empty())
        return true;
    auto res = std::find_end(str.begin(), str.end(), val.begin(), val.end());
    if (res == str.end())
        return false;
    return std::distance(res, str.end()) == static_cast<int>(val.size());
}

inline std::string before(const std::string& str, const std::string& val)
{
    auto res = str.find(val);
    if (res == std::string::npos)
        return str;
    return std::string{str.begin(), str.begin() + static_cast<int>(res)};
}

inline std::string after(const std::string& str, const std::string& val)
{
    auto res = str.find(val);
    if (res == std::string::npos)
        return {""};
    return std::string{str.begin() + static_cast<int>(res + val.size()), str.end()};
}

inline std::string between(const std::string& str, const std::string& afterStr, const std::string& beforeStr)
{
    return before(after(str, afterStr), beforeStr);
}

}
}

