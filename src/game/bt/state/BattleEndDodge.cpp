#include "game/bt/state/BattleEndDodge.hpp"

#include "game/bt/state/BattleStateType.hpp"

namespace ut::game::bt::state
{

BattleEndDodge::BattleEndDodge(scene::IngameBattle& scene) : BattleState(scene)
{
}

auto BattleEndDodge::handleInput() -> BattleStateType
{
    return BattleStateType::NONE;
}

auto BattleEndDodge::update() -> BattleStateType
{
    return BattleStateType::NONE;
}

} // namespace ut::game::bt::state
