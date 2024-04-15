#pragma once

#include <bn_algorithm.h>

#include "game/bt/mob/FroggitReact.hpp"
#include "game/bt/mob/MoldsmalReact.hpp"
#include "game/bt/mob/WhimsunReact.hpp"

namespace ut::game::bt::mob
{

inline constexpr int MOB_REACT_ALIGN_SIZE =
    bn::max({alignof(FroggitReact), alignof(WhimsunReact), alignof(MoldsmalReact)});
inline constexpr int MOB_REACT_MAX_SIZE = bn::max({sizeof(FroggitReact), sizeof(WhimsunReact), sizeof(MoldsmalReact)});

} // namespace ut::game::bt::mob
