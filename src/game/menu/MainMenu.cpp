#include "game/menu/MainMenu.hpp"

#include <bn_display.h>
#include <bn_keypad.h>
#include <bn_sound_item.h>
#include <bn_sprite_ptr.h>

#include "asset/SfxKind.hpp"
#include "game/menu/MenuStateType.hpp"
#include "scene/IngameMenu.hpp"

#include "bn_regular_bg_items_bg_ingame_menu.h"

namespace ut::game::menu
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = -bn::fixed_point{bn::display::width() / 2, bn::display::height() / 2};

constexpr bn::fixed_point CURSOR_POSS[3] = {
    bn::fixed_point{17, 58} + TOP_LEFT_ORIGIN,
    bn::fixed_point{17, 72} + TOP_LEFT_ORIGIN,
    bn::fixed_point{17, 86} + TOP_LEFT_ORIGIN,
};

} // namespace

MainMenu::MainMenu(scene::IngameMenu& scene) : MenuState(scene)
{
    asset::getSfx(asset::SfxKind::MENU_CURSOR)->play();

    scene._bg.set_item(bn::regular_bg_items::bg_ingame_menu, scene.isDialogUpper()
                                                                 ? scene::IngameMenu::BgMapIdx::MAIN_L
                                                                 : scene::IngameMenu::BgMapIdx::MAIN_U);
    moveCursor(false);
}

auto MainMenu::handleInput() -> MenuStateType
{
    if (bn::keypad::b_pressed() || bn::keypad::start_pressed() || bn::keypad::l_pressed() || bn::keypad::r_pressed())
        return MenuStateType::CLOSE_MENU;

    int& cursorIdx = _scene.getContext().menuCursorIdx;

    if (bn::keypad::a_pressed() && _scene._menuItemCount)
    {
        const auto& sfx = asset::getSfx(asset::SfxKind::MENU_ACTIVATE);

        if (cursorIdx == 0 && _scene.isItemMenuEnabled())
        {
            sfx->play();
            return MenuStateType::ITEM;
        }
        else if (cursorIdx == 1)
        {
            sfx->play();
            return MenuStateType::STAT;
        }
        else if (cursorIdx == 2 && _scene.isCellMenuEnabled())
        {
            sfx->play();
            return MenuStateType::CELL;
        }
    }
    else if (bn::keypad::up_pressed() && cursorIdx > 0)
    {
        cursorIdx -= 1;
        moveCursor(true);
    }
    else if (bn::keypad::down_pressed() && cursorIdx < _scene._menuItemCount - 1)
    {
        cursorIdx += 1;
        moveCursor(true);
    }

    return MenuStateType::NONE;
}

auto MainMenu::update() -> MenuStateType
{
    return MenuStateType::NONE;
}

void MainMenu::moveCursor(bool playSfx)
{
    if (playSfx)
        asset::getSfx(asset::SfxKind::MENU_CURSOR)->play();

    const auto menuDiff = (_scene.isDialogUpper() ? scene::IngameMenu::MENU_LOWER_DIFF : scene::IngameMenu::ZERO_DIFF);
    _scene._cursor.set_position(CURSOR_POSS[_scene.getContext().menuCursorIdx] + menuDiff);
}

} // namespace ut::game::menu
