#include "game/bt/state/BattleMenu.hpp"

#include "game/bt/state/BattleStateType.hpp"

namespace ut::game::bt::state
{

BattleMenu::BattleMenu(scene::IngameBattle& scene) : BattleState(scene)
{
}

auto BattleMenu::handleInput() -> BattleStateType
{
    return BattleStateType::NONE;
}

auto BattleMenu::update() -> BattleStateType
{
    return BattleStateType::NONE;
}

} // namespace ut::game::bt::state
