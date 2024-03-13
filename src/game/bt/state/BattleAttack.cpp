#include "game/bt/state/BattleAttack.hpp"

#include "game/bt/state/BattleStateType.hpp"

namespace ut::game::bt::state
{

BattleAttack::BattleAttack(scene::IngameBattle& scene) : BattleState(scene)
{
}

auto BattleAttack::handleInput() -> BattleStateType
{
    return BattleStateType::NONE;
}

auto BattleAttack::update() -> BattleStateType
{
    return BattleStateType::NONE;
}

} // namespace ut::game::bt::state
