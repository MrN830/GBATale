#include "asset/SpriteAnimInfo.hpp"

#include "asset/SpriteAnimKind.hpp"
#include "core/Directions.hpp"

#include "bn_sprite_items_ch_frisk_base.h"
#include "bn_sprite_items_ch_toriel.h"

namespace ut::asset
{

namespace
{

using Dirs = core::Directions;

constexpr int CHARACTER_WAIT_UPDATE = 5 - 1;

constexpr SpriteAnimInfo<4> frisk_walk_up{
    bn::sprite_items::ch_frisk_base,
    Dirs::UP,
    bn::fixed_point{0, 0},
    CHARACTER_WAIT_UPDATE,
    true,
    false,
    false,
    false,
    bn::array<uint16_t, 4>{5, 4, 3, 4},
};
constexpr SpriteAnimInfo<4> frisk_walk_down{
    bn::sprite_items::ch_frisk_base,
    Dirs::DOWN,
    bn::fixed_point{0, 0},
    CHARACTER_WAIT_UPDATE,
    true,
    false,
    false,
    false,
    bn::array<uint16_t, 4>{0, 1, 2, 1},
};
constexpr SpriteAnimInfo<2> frisk_walk_left{
    bn::sprite_items::ch_frisk_base,
    Dirs::LEFT,
    bn::fixed_point{0, 0},
    CHARACTER_WAIT_UPDATE,
    true,
    false,
    false,
    false,
    bn::array<uint16_t, 2>{7, 6},
};
constexpr SpriteAnimInfo<2> frisk_walk_right{
    bn::sprite_items::ch_frisk_base,
    Dirs::RIGHT,
    bn::fixed_point{0, 0},
    CHARACTER_WAIT_UPDATE,
    true,
    false,
    true,
    false,
    bn::array<uint16_t, 2>{7, 6},
};

constexpr SpriteAnimInfo<4> toriel_walk_up{
    bn::sprite_items::ch_toriel,
    Dirs::UP,
    bn::fixed_point{0, 0},
    CHARACTER_WAIT_UPDATE,
    true,
    false,
    false,
    false,
    bn::array<uint16_t, 4>{0, 1, 2, 1},
};
constexpr SpriteAnimInfo<4> toriel_walk_down{
    bn::sprite_items::ch_toriel,
    Dirs::DOWN,
    bn::fixed_point{0, 0},
    CHARACTER_WAIT_UPDATE,
    true,
    false,
    false,
    false,
    bn::array<uint16_t, 4>{6, 7, 8, 7},
};
constexpr SpriteAnimInfo<4> toriel_walk_left{
    bn::sprite_items::ch_toriel,
    Dirs::LEFT,
    bn::fixed_point{0, 0},
    CHARACTER_WAIT_UPDATE,
    true,
    false,
    false,
    false,
    bn::array<uint16_t, 4>{18, 19, 20, 19},
};
constexpr SpriteAnimInfo<4> toriel_walk_right{
    bn::sprite_items::ch_toriel,
    Dirs::RIGHT,
    bn::fixed_point{0, 0},
    CHARACTER_WAIT_UPDATE,
    true,
    false,
    true,
    false,
    bn::array<uint16_t, 4>{18, 19, 20, 19},
};

} // namespace

auto ISpriteAnimInfo::get(SpriteAnimKind kind) -> const ISpriteAnimInfo&
{
    switch (kind)
    {
    case SpriteAnimKind::FRISK_WALK_UP:
        return frisk_walk_up;
    case SpriteAnimKind::FRISK_WALK_DOWN:
        return frisk_walk_down;
    case SpriteAnimKind::FRISK_WALK_LEFT:
        return frisk_walk_left;
    case SpriteAnimKind::FRISK_WALK_RIGHT:
        return frisk_walk_right;

    case SpriteAnimKind::TORIEL_WALK_UP:
        return toriel_walk_up;
    case SpriteAnimKind::TORIEL_WALK_DOWN:
        return toriel_walk_down;
    case SpriteAnimKind::TORIEL_WALK_LEFT:
        return toriel_walk_left;
    case SpriteAnimKind::TORIEL_WALK_RIGHT:
        return toriel_walk_right;

    default:
        BN_ERROR("Invalid SpriteAnimKind=", (int)kind);
    }

    return frisk_walk_down;
}

} // namespace ut::asset
