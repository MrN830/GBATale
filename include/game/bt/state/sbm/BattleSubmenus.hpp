#pragma once

#include <bn_algorithm.h>

// #include "game/bt/state/sbm/BattleSubmenuAct.hpp"
#include "game/bt/state/sbm/BattleSubmenuFight.hpp"
// #include "game/bt/state/sbm/BattleSubmenuItem.hpp"
#include "game/bt/state/sbm/BattleSubmenuMain.hpp"
// #include "game/bt/state/sbm/BattleSubmenuMercy.hpp"

namespace ut::game::bt::state::sbm
{

inline constexpr int BT_SBM_ALIGN_SIZE = bn::max({
    alignof(BattleSubmenuMain), alignof(BattleSubmenuFight),
    // alignof(BattleSubmenuAct),
    // alignof(BattleSubmenuItem),
    // alignof(BattleSubmenuMercy),
});
inline constexpr int BT_SBM_MAX_SIZE = bn::max({
    sizeof(BattleSubmenuMain), sizeof(BattleSubmenuFight),
    // sizeof(BattleSubmenuAct),
    // sizeof(BattleSubmenuItem),
    // sizeof(BattleSubmenuMercy),
});

} // namespace ut::game::bt::state::sbm
