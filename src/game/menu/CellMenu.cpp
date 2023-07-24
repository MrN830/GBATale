#include "game/menu/CellMenu.hpp"

#include <bn_keypad.h>

#include "asset/FontKind.hpp"
#include "core/TextGens.hpp"
#include "game/menu/MenuStateType.hpp"
#include "scene/IngameMenu.hpp"

#include "bn_regular_bg_items_bg_ingame_menu.h"

namespace ut::game::menu
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = -bn::fixed_point{bn::display::width() / 2, bn::display::height() / 2};

constexpr auto START_POS = bn::fixed_point{80, 16} + TOP_LEFT_ORIGIN;

} // namespace

CellMenu::CellMenu(scene::IngameMenu& scene) : MenuState(scene)
{
    scene._bg.set_item(bn::regular_bg_items::bg_ingame_menu, scene.isDialogUpper()
                                                                 ? scene::IngameMenu::BgMapIdx::CELL_L
                                                                 : scene::IngameMenu::BgMapIdx::CELL_U);
    scene._cursor.set_visible(false);

    auto& textGen = scene.getContext().textGens.get(asset::FontKind::MAIN);
    textGen.generate(START_POS, "Not Implemented :(", _text);
    for (auto& spr : _text)
        spr.set_bg_priority(scene::IngameMenu::BG_PRIORITY);
}

CellMenu::~CellMenu()
{
    _scene._cursor.set_visible(true);
}

auto CellMenu::handleInput() -> MenuStateType
{
    if (bn::keypad::b_pressed())
        return MenuStateType::MAIN;

    return MenuStateType::NONE;
}

auto CellMenu::update() -> MenuStateType
{
    return MenuStateType::NONE;
}

} // namespace ut::game::menu
