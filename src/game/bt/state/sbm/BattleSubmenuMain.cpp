#include "game/bt/state/sbm/BattleSubmenuMain.hpp"

#include <bn_display.h>
#include <bn_keypad.h>
#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "game/bt/state/BattleMenu.hpp"
#include "game/bt/state/BattleStateType.hpp"
#include "game/bt/state/sbm/BattleSubmenuType.hpp"
#include "scene/IngameBattle.hpp"

#include "game/bt/bt_consts.hpp"

#include "bn_sprite_items_ui_battle_buttons.h"

namespace ut::game::bt::state::sbm
{

namespace
{

enum ButtonIdx
{
    FIGHT = 0,
    ACT,
    ITEM,
    MERCY
};

constexpr bn::fixed_point BTN_POS_DIFF = {32 - bn::display::width() / 2, 16 - bn::display::height() / 2};

constexpr bn::array<bn::fixed_point, 4> BTN_POSS = {
    bn::fixed_point{2, 2} + BTN_POS_DIFF,
    bn::fixed_point{67, 2} + BTN_POS_DIFF,
    bn::fixed_point{133, 2} + BTN_POS_DIFF,
    bn::fixed_point{198, 2} + BTN_POS_DIFF,
};

}; // namespace

BattleSubmenuMain::BattleSubmenuMain(BattleMenu& priMenu) : BattleSubmenu(priMenu)
{
    priMenu.getScene().getTopUI().setVisible(false);

    for (int i = 0; i < 4; ++i)
    {
        _buttons.push_back(bn::sprite_items::ui_battle_buttons.create_sprite(BTN_POSS[i], i * 2));
        _buttons.back().set_bg_priority(consts::BG_BOX_PRIORITY);
    }

    // highlight selected button
    const int cursorIdx = getCursorIdx();
    _buttons[cursorIdx].set_tiles(bn::sprite_items::ui_battle_buttons.tiles_item(), cursorIdx * 2 + 1);
}

BattleSubmenuMain::~BattleSubmenuMain()
{
    _priMenu.getScene().getTopUI().setVisible(true);
}

auto BattleSubmenuMain::handleInput() -> BattleSubmenuType
{
    if (bn::keypad::a_pressed())
        return activate();
    else if (bn::keypad::left_pressed())
        select((getCursorIdx() - 1 + _buttons.size()) % _buttons.size());
    else if (bn::keypad::right_pressed())
        select((getCursorIdx() + 1) % _buttons.size());

    return BattleSubmenuType::NONE;
}

void BattleSubmenuMain::select(int idx)
{
    BN_ASSERT(0 <= idx && idx < _buttons.size());

    asset::getSfx(asset::SfxKind::MENU_CURSOR)->play();

    // move highlight
    const int cursorIdx = getCursorIdx();
    _buttons[cursorIdx].set_tiles(bn::sprite_items::ui_battle_buttons.tiles_item(), cursorIdx * 2);
    _buttons[idx].set_tiles(bn::sprite_items::ui_battle_buttons.tiles_item(), idx * 2 + 1);

    setCursorIdx(idx);
}

auto BattleSubmenuMain::activate() const -> BattleSubmenuType
{
    asset::getSfx(asset::SfxKind::MENU_ACTIVATE)->play();

    switch (getCursorIdx())
    {
    case ButtonIdx::FIGHT:
        return BattleSubmenuType::FIGHT;
    case ButtonIdx::ACT:
        return BattleSubmenuType::ACT;
    case ButtonIdx::ITEM:
        return BattleSubmenuType::ITEM;
    case ButtonIdx::MERCY:
        return BattleSubmenuType::MERCY;

    default:
        BN_ERROR("Invalid cursurIdx=", getCursorIdx());
    }

    return BattleSubmenuType::NONE;
}

int BattleSubmenuMain::getCursorIdx() const
{
    return _priMenu.getScene().getBtTempVars().submenuMainCursorIdx;
}

void BattleSubmenuMain::setCursorIdx(int idx)
{
    _priMenu.getScene().getBtTempVars().submenuMainCursorIdx = idx;
}

} // namespace ut::game::bt::state::sbm
