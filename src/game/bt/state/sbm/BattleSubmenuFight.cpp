#include "game/bt/state/sbm/BattleSubmenuFight.hpp"

#include <bn_keypad.h>

#include "game/bt/state/BattleMenu.hpp"
#include "game/bt/state/BattleStateType.hpp"
#include "game/bt/state/sbm/BattleSubmenuType.hpp"

namespace ut::game::bt::state::sbm
{

BattleSubmenuFight::BattleSubmenuFight(BattleMenu& priMenu) : BattleSubmenu(priMenu)
{
}

BattleSubmenuFight::~BattleSubmenuFight()
{
}

auto BattleSubmenuFight::handleInput() -> BattleSubmenuType
{
    // test
    if (bn::keypad::a_pressed())
        _priMenu.setNextBattleState(BattleStateType::BATTLE_PREPARE_DODGE);

    return BattleSubmenuType::NONE;
}

} // namespace ut::game::bt::state::sbm
