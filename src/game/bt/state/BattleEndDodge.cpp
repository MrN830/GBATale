#include "game/bt/state/BattleEndDodge.hpp"

#include "game/bt/state/BattleStateType.hpp"

#include "core/MovingBgBox.hpp"
#include "scene/IngameBattle.hpp"

#include "game/bt/bt_consts.hpp"

namespace ut::game::bt::state
{

BattleEndDodge::BattleEndDodge(scene::IngameBattle& scene) : BattleState(scene)
{
    _scene.getMovingBgBox().moveTo(BG_BOX_INIT_RECT);
}

auto BattleEndDodge::handleInput() -> BattleStateType
{
    return BattleStateType::NONE;
}

auto BattleEndDodge::update() -> BattleStateType
{
    if (_scene.getMovingBgBox().isDoneMoving())
        return BattleStateType::BATTLE_MENU;

    return BattleStateType::NONE;
}

} // namespace ut::game::bt::state
