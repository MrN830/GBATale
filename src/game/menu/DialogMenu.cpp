#include "game/menu/DialogMenu.hpp"

#include <bn_keypad.h>

#include "game/ItemInfo.hpp"
#include "game/menu/MenuStateType.hpp"
#include "scene/IngameMenu.hpp"

#include "bn_regular_bg_items_bg_ingame_menu.h"

namespace ut::game::menu
{

DialogMenu::DialogMenu(scene::IngameMenu& scene)
    : MenuState(scene), _dialogWriter(scene.getContext().textGens, scene::IngameMenu::BG_PRIORITY)
{
    BN_ASSERT(!scene._dialogs.empty(), "DialogMenu with empty dialogs");

    scene._bg.set_item(bn::regular_bg_items::bg_ingame_menu, scene.isDialogUpper()
                                                                 ? scene::IngameMenu::BgMapIdx::DIALOG_L
                                                                 : scene::IngameMenu::BgMapIdx::DIALOG_U);
    scene._cursor.set_visible(false);

    using DS = core::DialogSettings;
    using DSPKind = DS::PresetKind;
    const auto& dialogSettings = DS::getPreset(scene.isDialogUpper() ? DSPKind::WORLD_UPPER : DSPKind::WORLD_LOWER);

    for (const auto& msg : _scene._dialogs)
        _dialogs.push_back(msg);

    _dialogWriter.start(bn::span(_dialogs.cbegin(), _dialogs.cend()), dialogSettings, _text);
}

auto DialogMenu::handleInput() -> MenuStateType
{
    if (bn::keypad::a_pressed())
        _dialogWriter.confirmKeyInput();
    if (bn::keypad::b_pressed())
        _dialogWriter.instantWrite();

    return MenuStateType::NONE;
}

auto DialogMenu::update() -> MenuStateType
{
    _dialogWriter.update();

    if (!_dialogWriter.isStarted())
        return MenuStateType::CLOSE_MENU;

    return MenuStateType::NONE;
}

} // namespace ut::game::menu
