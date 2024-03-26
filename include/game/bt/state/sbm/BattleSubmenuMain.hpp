#pragma once

#include "game/bt/state/sbm/BattleSubmenu.hpp"

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "core/DialogWriter.hpp"

#include "consts.hpp"

namespace ut::game::bt::state::sbm
{

class BattleSubmenuMain final : public BattleSubmenu
{
public:
    BattleSubmenuMain(BattleMenu& priMenu);
    ~BattleSubmenuMain();

    auto handleInput() -> BattleSubmenuType override;
    auto update() -> BattleSubmenuType override;

private:
    void select(int idx);
    auto activate() const -> BattleSubmenuType;

private:
    int getCursorIdx() const;
    void setCursorIdx(int idx);

private:
    bn::vector<bn::sprite_ptr, 4> _buttons;

    bn::vector<bn::sprite_ptr, ut::consts::DIALOG_MAX_SPRITES> _dialogSprs;
    core::DialogWriter _dialogWriter;
};

} // namespace ut::game::bt::state::sbm
