#include "game/bt/state/BattlePrepareDodge.hpp"

#include "game/bt/state/BattleStateType.hpp"

namespace ut::game::bt::state
{

BattlePrepareDodge::BattlePrepareDodge(scene::IngameBattle& scene) : BattleState(scene)
{
}

auto BattlePrepareDodge::handleInput() -> BattleStateType
{
    return BattleStateType::NONE;
}

auto BattlePrepareDodge::update() -> BattleStateType
{
    return BattleStateType::NONE;
}

} // namespace ut::game::bt::state
