#pragma once

#include <cstdint>

namespace ut::asset
{

enum class SpriteAnimKind : int16_t
{
    NONE = -1,

    FRISK_WALK_UP,
    FRISK_WALK_DOWN,
    FRISK_WALK_LEFT,
    FRISK_WALK_RIGHT,

    TORIEL_WALK_UP,
    TORIEL_WALK_DOWN,
    TORIEL_WALK_LEFT,
    TORIEL_WALK_RIGHT,

    TOTAL_COUNT
};

} // namespace ut::asset
