#pragma once

#include <bn_algorithm.h>

#include "game/bt/mob/FroggitAnim.hpp"
#include "game/bt/mob/MoldsmalAnim.hpp"
#include "game/bt/mob/WhimsunAnim.hpp"

namespace ut::game::bt::mob
{

inline constexpr int MOB_ANIM_ALIGN_SIZE = bn::max({alignof(FroggitAnim), alignof(WhimsunAnim), alignof(MoldsmalAnim)});
inline constexpr int MOB_ANIM_MAX_SIZE = bn::max({sizeof(FroggitAnim), sizeof(WhimsunAnim), sizeof(MoldsmalAnim)});

} // namespace ut::game::bt::mob
