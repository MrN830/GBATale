#pragma once

#include <cstdint>

namespace ut::mtile
{

enum class WarpId : int8_t
{
    NONE = -1,
    INIT = 0,
    A,
    B,
    C,
    D,
    X,

    TOTAL_COUNT
};

} // namespace ut::mtile
