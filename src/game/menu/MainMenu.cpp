#include "game/menu/MainMenu.hpp"

#include <bn_display.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_sound_item.h>
#include <bn_sprite_ptr.h>

#include "asset/SfxKind.hpp"
#include "game/menu/MenuStateType.hpp"
#include "scene/IngameMenu.hpp"

#include "bn_regular_bg_items_bg_ingame_menu_main.h"

namespace ut::game::menu
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = -bn::fixed_point{bn::display::width() / 2, bn::display::height() / 2};

constexpr bn::fixed_point CURSOR_POSS[3] = {
    bn::fixed_point{14, 58} + TOP_LEFT_ORIGIN,
    bn::fixed_point{14, 72} + TOP_LEFT_ORIGIN,
    bn::fixed_point{14, 86} + TOP_LEFT_ORIGIN,
};

} // namespace

MainMenu::MainMenu(scene::IngameMenu& scene)
{
    scene._bg.set_item(bn::regular_bg_items::bg_ingame_menu_main);
    scene._cursor.set_position(CURSOR_POSS[scene.getContext().menuCursorIdx]);
}

auto MainMenu::handleInput(scene::IngameMenu& scene) -> MenuStateType
{
    if (bn::keypad::b_pressed() || bn::keypad::start_pressed() || bn::keypad::l_pressed() || bn::keypad::r_pressed())
        return MenuStateType::CLOSE_MENU;

    int& cursorIdx = scene.getContext().menuCursorIdx;

    if (bn::keypad::up_pressed() && cursorIdx > 0)
    {
        cursorIdx -= 1;
        moveCursor(scene._cursor, scene);
    }
    else if (bn::keypad::down_pressed() && cursorIdx < scene._menuItemCount - 1)
    {
        cursorIdx += 1;
        moveCursor(scene._cursor, scene);
    }

    return MenuStateType::NONE;
}

auto MainMenu::update(scene::IngameMenu&) -> MenuStateType
{
    return MenuStateType::NONE;
}

void MainMenu::moveCursor(bn::sprite_ptr& cursor, scene::IngameMenu& scene)
{
    asset::getSfx(asset::SfxKind::MENU_CURSOR)->play();

    cursor.set_position(CURSOR_POSS[scene.getContext().menuCursorIdx]);
}

} // namespace ut::game::menu
