#pragma once

#include "game/bt/state/sbm/BattleSubmenu.hpp"

namespace ut::game::bt::state::sbm
{

class BattleSubmenuFight final : public BattleSubmenu
{
public:
    BattleSubmenuFight(BattleMenu& priMenu);
    ~BattleSubmenuFight();

    auto handleInput() -> BattleSubmenuType override;
};

} // namespace ut::game::bt::state::sbm
