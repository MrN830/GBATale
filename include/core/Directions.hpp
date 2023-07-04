#pragma once

#include <bn_fixed_point.h>

#include "util/enum_as_flags.hpp"

namespace ut::core
{

enum class Directions : uint8_t
{
    NONE = 0,

    UP = 1 << 0,
    DOWN = 1 << 1,
    LEFT = 1 << 2,
    RIGHT = 1 << 3
};

ENUM_AS_FLAGS(Directions);

auto dirs2Pos(Directions dirs) -> bn::fixed_point;
auto pos2Dirs(const bn::fixed_point& pos) -> Directions;

} // namespace ut::core
