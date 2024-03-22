#pragma once

#include "game/bt/state/sbm/BattleSubmenu.hpp"

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace ut::game::bt::state::sbm
{

class BattleSubmenuMain final : public BattleSubmenu
{
public:
    BattleSubmenuMain(BattleMenu& priMenu);

    auto handleInput() -> BattleSubmenuType override;

private:
    void select(int idx);
    auto activate() const -> BattleSubmenuType;

private:
    int getCursorIdx() const;
    void setCursorIdx(int idx);

private:
    bn::vector<bn::sprite_ptr, 4> _buttons;
};

} // namespace ut::game::bt::state::sbm
