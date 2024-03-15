#include "game/bt/state/BattleMenu.hpp"

#include <bn_keypad.h>

#include "game/bt/state/BattleStateType.hpp"

namespace ut::game::bt::state
{

BattleMenu::BattleMenu(scene::IngameBattle& scene) : BattleState(scene)
{
}

auto BattleMenu::handleInput() -> BattleStateType
{
    // test
    if (bn::keypad::a_pressed())
        return BattleStateType::BATTLE_PREPARE_DODGE;

    return BattleStateType::NONE;
}

auto BattleMenu::update() -> BattleStateType
{
    return BattleStateType::NONE;
}

} // namespace ut::game::bt::state
