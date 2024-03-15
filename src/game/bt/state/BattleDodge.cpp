#include "game/bt/state/BattleDodge.hpp"

#include <bn_keypad.h> // test

#include "game/bt/state/BattleStateType.hpp"

namespace ut::game::bt::state
{

BattleDodge::BattleDodge(scene::IngameBattle& scene) : BattleState(scene)
{
}

auto BattleDodge::handleInput() -> BattleStateType
{
    // test
    if (bn::keypad::a_pressed())
        return BattleStateType::BATTLE_END_DODGE;

    return BattleStateType::NONE;
}

auto BattleDodge::update() -> BattleStateType
{
    return BattleStateType::NONE;
}

} // namespace ut::game::bt::state
