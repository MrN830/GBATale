#include "game/bt/state/BattleAttack.hpp"

#include <bn_keypad.h>

#include "game/bt/state/BattleStateType.hpp"
#include "scene/IngameBattle.hpp"

namespace ut::game::bt::state
{

BattleAttack::BattleAttack(scene::IngameBattle& scene) : BattleState(scene)
{
    scene.getAttackBg().setVisible(true);
}

auto BattleAttack::handleInput() -> BattleStateType
{
    // test
    if (bn::keypad::a_pressed())
        return BattleStateType::BATTLE_PREPARE_DODGE;

    return BattleStateType::NONE;
}

auto BattleAttack::update() -> BattleStateType
{
    return BattleStateType::NONE;
}

} // namespace ut::game::bt::state
