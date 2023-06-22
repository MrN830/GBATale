#include "game/menu/StatMenu.hpp"

#include "game/menu/MenuStateType.hpp"
#include "scene/IngameMenu.hpp"

#include "bn_regular_bg_items_bg_ingame_menu_stat.h"

namespace ut::game::menu
{

StatMenu::StatMenu(scene::IngameMenu& scene)
{
    scene._bg.set_item(bn::regular_bg_items::bg_ingame_menu_stat);
}

auto StatMenu::handleInput(scene::IngameMenu& scene) -> MenuStateType
{
    return MenuStateType::NONE;
}

auto StatMenu::update(scene::IngameMenu& scene) -> MenuStateType
{
    return MenuStateType::NONE;
}

} // namespace ut::game::menu
