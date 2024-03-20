#pragma once

#include <bn_algorithm.h>

#include "game/bt/mob/WhimsunAnim.hpp"

namespace ut::game::bt::mob
{

inline constexpr int MOB_ANIM_ALIGN_SIZE = bn::max({alignof(WhimsunAnim)});
inline constexpr int MOB_ANIM_MAX_SIZE = bn::max({sizeof(WhimsunAnim)});

} // namespace ut::game::bt::mob
