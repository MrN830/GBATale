#include "game/bt/state/BattlePrepareDodge.hpp"

#include <bn_display.h> // test

#include "game/bt/state/BattleStateType.hpp"

#include "core/MovingBgBox.hpp"
#include "scene/IngameBattle.hpp"

namespace ut::game::bt::state
{

BattlePrepareDodge::BattlePrepareDodge(scene::IngameBattle& scene) : BattleState(scene)
{
    _scene.getMovingBgBox().moveTo({79 - bn::display::width() / 2, 88 - bn::display::height() / 2, 82, 70}); // test
}

auto BattlePrepareDodge::handleInput() -> BattleStateType
{
    return BattleStateType::NONE;
}

auto BattlePrepareDodge::update() -> BattleStateType
{
    // TODO: change to mob text done
    if (_scene.getMovingBgBox().isDoneMoving())
        return BattleStateType::BATTLE_DODGE;

    return BattleStateType::NONE;
}

} // namespace ut::game::bt::state
