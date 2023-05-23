#pragma once

#include <bn_string.h>

namespace ut::util
{

/**
 * @brief Convert a ASCII string to a Uppercase.
 * Note that this is unsafe for UTF-8 string.
 */
template <int MaxSize>
[[nodiscard]] auto toUpperAscii(const bn::string<MaxSize>& str) -> bn::string<MaxSize>
{
    bn::string<MaxSize> result;

    for (char ch : str)
    {
        if ('a' <= ch && ch <= 'z')
            ch = 'A' + ch - 'a';

        result.push_back(ch);
    }

    return result;
}

/**
 * @brief Convert a ASCII string to a lowercase.
 * Note that this is unsafe for UTF-8 string.
 */
template <int MaxSize>
[[nodiscard]] auto toLowerAscii(const bn::string<MaxSize>& str) -> bn::string<MaxSize>
{
    bn::string<MaxSize> result;

    for (char ch : str)
    {
        if ('A' <= ch && ch <= 'Z')
            ch = 'a' + ch - 'A';

        result.push_back(ch);
    }

    return result;
}

} // namespace ut::util
