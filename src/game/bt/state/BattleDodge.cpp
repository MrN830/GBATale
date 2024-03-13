#include "game/bt/state/BattleDodge.hpp"

#include "game/bt/state/BattleStateType.hpp"

namespace ut::game::bt::state
{

BattleDodge::BattleDodge(scene::IngameBattle& scene) : BattleState(scene)
{
}

auto BattleDodge::handleInput() -> BattleStateType
{
    return BattleStateType::NONE;
}

auto BattleDodge::update() -> BattleStateType
{
    return BattleStateType::NONE;
}

} // namespace ut::game::bt::state
