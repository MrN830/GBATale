#pragma once

#include <bn_algorithm.h>

#include "game/bt/state/BattleAttack.hpp"
#include "game/bt/state/BattleDodge.hpp"
#include "game/bt/state/BattleEndDodge.hpp"
#include "game/bt/state/BattleMenu.hpp"
#include "game/bt/state/BattlePrepareDodge.hpp"

namespace ut::game::bt::state
{

inline constexpr int BT_ST_ALIGN_SIZE =
    bn::max({alignof(BattleMenu), alignof(BattleAttack), alignof(BattlePrepareDodge), alignof(BattleDodge),
             alignof(BattleEndDodge)});
inline constexpr int BT_ST_MAX_SIZE = bn::max({sizeof(BattleMenu), sizeof(BattleAttack), sizeof(BattlePrepareDodge),
                                               sizeof(BattleDodge), sizeof(BattleEndDodge)});

} // namespace ut::game::bt::state
