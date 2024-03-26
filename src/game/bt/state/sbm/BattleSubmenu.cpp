#include "game/bt/state/sbm/BattleSubmenu.hpp"

#include "game/bt/state/sbm/BattleSubmenuType.hpp"

namespace ut::game::bt::state::sbm
{

auto BattleSubmenu::update() -> BattleSubmenuType
{
    return BattleSubmenuType::NONE;
}

} // namespace ut::game::bt::state::sbm
