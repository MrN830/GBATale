#pragma once

#include <cstdint>

namespace ut::asset
{

enum class SpriteAnimKind : int16_t
{
    NONE = -1,

    // normal entities
    SAVE_POINT,
    FROGGIT,
    STALKER_FLOWEY,

    // animated terrain
    FAKE_WATER_L,
    FAKE_WATER_R,

    FAKE_WATER_SHADOW_L,
    FAKE_WATER_SHADOW_M,
    FAKE_WATER_SHADOW_R,

    FAKE_WATER_OPEN_L,
    FAKE_WATER_OPEN_M,
    FAKE_WATER_OPEN_R,

    FAKE_WATER_SHADOW_M_4X,
    FAKE_WATER_OPEN_M_4X,

    // individual walk `SpriteAnimKind`s
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

enum class WalkAnimKind : int8_t
{
    NONE = -1,

    FRISK,
    TORIEL,

    TOTAL_COUNT
};

} // namespace ut::asset
