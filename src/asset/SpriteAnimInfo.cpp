#include "asset/SpriteAnimInfo.hpp"

#include "asset/SpriteAnimKind.hpp"
#include "core/Directions.hpp"

#include "bn_sprite_items_ch_flowey.h"
#include "bn_sprite_items_ch_frisk_base.h"
#include "bn_sprite_items_ch_toriel.h"
#include "bn_sprite_items_ent_froggit.h"
#include "bn_sprite_items_ent_save_point.h"
#include "bn_sprite_items_tr_fake_water.h"
#include "bn_sprite_items_tr_fake_water_4x.h"
#include "bn_sprite_items_tr_fake_water_open.h"

namespace ut::asset
{

namespace
{

using Dirs = core::Directions;

constexpr int CHARACTER_WAIT_UPDATE = 5 - 1;

// normal entities
constexpr SpriteAnimInfo<2> save_point{
    bn::sprite_items::ent_save_point, Dirs::NONE, bn::fixed_point{0, 0}, 5 - 1, true, false, false, false,
    bn::array<uint16_t, 2>{0, 1},
};
constexpr SpriteAnimInfo<4> froggit{
    bn::sprite_items::ent_froggit,      Dirs::NONE, bn::fixed_point{0, 0}, 10 - 1, true, false, false, false,
    bn::array<uint16_t, 4>{0, 1, 2, 1},
};
constexpr SpriteAnimInfo<4> stalker_flowey{
    bn::sprite_items::ch_flowey,           Dirs::NONE, bn::fixed_point{0, 0}, 2 - 1, false, false, false, false,
    bn::array<uint16_t, 4>{0, 14, 15, 16},
};

// animated terrain
constexpr SpriteAnimInfo<3> fake_water_l{
    bn::sprite_items::tr_fake_water, Dirs::NONE, bn::fixed_point{0, 0}, 8 - 1, true, false, false, false,
    bn::array<uint16_t, 3>{0, 1, 2},
};
constexpr SpriteAnimInfo<3> fake_water_r{
    bn::sprite_items::tr_fake_water, Dirs::NONE, bn::fixed_point{0, 0}, 8 - 1, true, false, false, false,
    bn::array<uint16_t, 3>{3, 4, 5},
};
constexpr SpriteAnimInfo<3> fake_water_shadow_l{
    bn::sprite_items::tr_fake_water, Dirs::NONE, bn::fixed_point{0, 0}, 8 - 1, true, false, false, false,
    bn::array<uint16_t, 3>{6, 7, 8},
};
constexpr SpriteAnimInfo<3> fake_water_shadow_m{
    bn::sprite_items::tr_fake_water,   Dirs::NONE, bn::fixed_point{0, 0}, 8 - 1, true, false, false, false,
    bn::array<uint16_t, 3>{9, 10, 11},
};
constexpr SpriteAnimInfo<3> fake_water_shadow_r{
    bn::sprite_items::tr_fake_water,    Dirs::NONE, bn::fixed_point{0, 0}, 8 - 1, true, false, false, false,
    bn::array<uint16_t, 3>{12, 13, 14},
};
constexpr SpriteAnimInfo<3> fake_water_open_l{
    bn::sprite_items::tr_fake_water_open, Dirs::NONE, bn::fixed_point{0, 0}, 8 - 1, true, false, false, false,
    bn::array<uint16_t, 3>{0, 1, 2},
};
constexpr SpriteAnimInfo<3> fake_water_open_m{
    bn::sprite_items::tr_fake_water_open, Dirs::NONE, bn::fixed_point{0, 0}, 8 - 1, true, false, false, false,
    bn::array<uint16_t, 3>{3, 4, 5},
};
constexpr SpriteAnimInfo<3> fake_water_open_r{
    bn::sprite_items::tr_fake_water_open, Dirs::NONE, bn::fixed_point{0, 0}, 8 - 1, true, false, false, false,
    bn::array<uint16_t, 3>{6, 7, 8},
};
constexpr SpriteAnimInfo<3> fake_water_shadow_m_4x{
    bn::sprite_items::tr_fake_water_4x, Dirs::NONE, bn::fixed_point{0, 0}, 8 - 1, true, false, false, false,
    bn::array<uint16_t, 3>{0, 1, 2},
};
constexpr SpriteAnimInfo<3> fake_water_open_m_4x{
    bn::sprite_items::tr_fake_water_4x, Dirs::NONE, bn::fixed_point{0, 0}, 8 - 1, true, false, false, false,
    bn::array<uint16_t, 3>{3, 4, 5},
};

// individual walk `SpriteAnimKind`s
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

constexpr const ISpriteAnimInfo* SPR_ANIMS[(int)SpriteAnimKind::TOTAL_COUNT] = {
    &save_point,        &froggit,           &stalker_flowey,

    &fake_water_l,      &fake_water_r,      &fake_water_shadow_l, &fake_water_shadow_m,    &fake_water_shadow_r,
    &fake_water_open_l, &fake_water_open_m, &fake_water_open_r,   &fake_water_shadow_m_4x, &fake_water_open_m_4x,

    &frisk_walk_up,     &frisk_walk_down,   &frisk_walk_left,     &frisk_walk_right,       &toriel_walk_up,
    &toriel_walk_down,  &toriel_walk_left,  &toriel_walk_right,
};

using SAKind = SpriteAnimKind;

constexpr const WalkAnimInfo WALK_ANIMS[(int)WalkAnimKind::TOTAL_COUNT] = {
    {SAKind::FRISK_WALK_UP, SAKind::FRISK_WALK_DOWN, SAKind::FRISK_WALK_LEFT, SAKind::FRISK_WALK_RIGHT},
    {SAKind::TORIEL_WALK_UP, SAKind::TORIEL_WALK_DOWN, SAKind::TORIEL_WALK_LEFT, SAKind::TORIEL_WALK_RIGHT},
};

} // namespace

auto ISpriteAnimInfo::get(SpriteAnimKind kind) -> const ISpriteAnimInfo&
{
    BN_ASSERT(0 <= (int)kind && (int)kind < (int)SpriteAnimKind::TOTAL_COUNT, "Invalid SpriteAnimKind=", (int)kind);

    return *SPR_ANIMS[(int)kind];
}

auto WalkAnimInfo::get(WalkAnimKind kind) -> const WalkAnimInfo&
{
    BN_ASSERT(0 <= (int)kind && (int)kind < (int)WalkAnimKind::TOTAL_COUNT, "Invalid WalkAnimKind=", (int)kind);

    return WALK_ANIMS[(int)kind];
}

} // namespace ut::asset
