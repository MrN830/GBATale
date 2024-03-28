#include "game/bt/state/BattleDodge.hpp"

#include <bn_keypad.h> // test

#include "game/bt/state/BattleStateType.hpp"
#include "scene/IngameBattle.hpp"

#include "game/bt/bt_consts.hpp"

namespace ut::game::bt::state
{

BattleDodge::BattleDodge(scene::IngameBattle& scene) : BattleState(scene)
{
    // TODO: change bg rect considering attack kind
    scene.getMovingBgBox().moveTo(consts::BG_BOX_DODGE_DEFAULT_RECT);
}

auto BattleDodge::handleInput() -> BattleStateType
{
    // test
    if (bn::keypad::a_pressed() && _scene.getMovingBgBox().isDoneMoving())
        return BattleStateType::BATTLE_END_DODGE;

    return BattleStateType::NONE;
}

auto BattleDodge::update() -> BattleStateType
{
    return BattleStateType::NONE;
}

} // namespace ut::game::bt::state
