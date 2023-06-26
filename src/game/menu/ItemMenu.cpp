#include "game/menu/ItemMenu.hpp"

#include <bn_display.h>
#include <bn_keypad.h>
#include <bn_sound_item.h>

#include "asset/FontKind.hpp"
#include "asset/SfxKind.hpp"
#include "core/TextGens.hpp"
#include "game/GameState.hpp"
#include "game/ItemActivator.hpp"
#include "game/ItemInfo.hpp"
#include "game/menu/MenuStateType.hpp"
#include "scene/IngameMenu.hpp"

#include "bn_regular_bg_items_bg_ingame_menu_item1.h"
#include "bn_regular_bg_items_bg_ingame_menu_item2.h"

namespace ut::game::menu
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = -bn::fixed_point{bn::display::width() / 2, bn::display::height() / 2};

constexpr auto ITEM_MARGIN = bn::fixed_point{0, 13};
constexpr auto CURSOR_DIFF = bn::fixed_point{-9, 0};

constexpr auto FIRST_ITEM_TEXT_POS = bn::fixed_point{90, 15} + TOP_LEFT_ORIGIN;
constexpr auto FIRST_ITEM_CURSOR_POS = FIRST_ITEM_TEXT_POS + CURSOR_DIFF;

constexpr auto USE_TEXT_POS = bn::fixed_point{90, 128} + TOP_LEFT_ORIGIN;
constexpr auto INFO_TEXT_POS = bn::fixed_point{135, 128} + TOP_LEFT_ORIGIN;
constexpr auto DROP_TEXT_POS = bn::fixed_point{190, 128} + TOP_LEFT_ORIGIN;

constexpr bn::fixed_point USAGE_CURSOR_POSS[3] = {
    USE_TEXT_POS + CURSOR_DIFF,
    INFO_TEXT_POS + CURSOR_DIFF,
    DROP_TEXT_POS + CURSOR_DIFF,
};

} // namespace

ItemMenu::ItemMenu(scene::IngameMenu& scene)
    : MenuState(scene), _itemCount(scene.getContext().gameState.getItems().size())
{
    BN_ASSERT(_itemCount > 0, "ItemMenu when no item in pocket");

    scene._bg.set_item(scene.isDialogUpper() ? bn::regular_bg_items::bg_ingame_menu_item2
                                             : bn::regular_bg_items::bg_ingame_menu_item1);
    moveCursorItem(false);

    auto& textGen = scene.getContext().textGens.get(asset::FontKind::MAIN);
    textGen.generate(USE_TEXT_POS, "USE", _text);
    textGen.generate(INFO_TEXT_POS, "INFO", _text);
    textGen.generate(DROP_TEXT_POS, "DROP", _text);

    const auto& items = scene.getContext().gameState.getItems();
    for (int i = 0; i < _itemCount; ++i)
    {
        const auto& item = ItemInfo::get(items[i]);
        textGen.generate(FIRST_ITEM_TEXT_POS + ITEM_MARGIN * i, item.getName(), _text);
    }

    for (auto& spr : _text)
        spr.set_bg_priority(scene::IngameMenu::BG_PRIORITY);
}

auto ItemMenu::handleInput() -> MenuStateType
{
    if (_itemMenuState == State::SELECT_ITEM)
        return handleItemSelect();
    else if (_itemMenuState == State::SELECT_USAGE)
        return handleUsageSelect();

    BN_ERROR("Should not reach here");
    return MenuStateType::NONE;
}

auto ItemMenu::update() -> MenuStateType
{
    return MenuStateType::NONE;
}

auto ItemMenu::handleItemSelect() -> MenuStateType
{
    if (bn::keypad::b_pressed())
    {
        return MenuStateType::MAIN;
    }
    else if (bn::keypad::a_pressed())
    {
        asset::getSfx(asset::SfxKind::MENU_ACTIVATE)->play();

        _itemMenuState = State::SELECT_USAGE;
        _usageIdx = 0;
        moveCursorUsage(false);
    }
    else if (bn::keypad::up_pressed() && _itemIdx > 0)
    {
        _itemIdx -= 1;
        moveCursorItem(true);
    }
    else if (bn::keypad::down_pressed() && _itemIdx < _itemCount - 1)
    {
        _itemIdx += 1;
        moveCursorItem(true);
    }

    return MenuStateType::NONE;
}

auto ItemMenu::handleUsageSelect() -> MenuStateType
{
    if (bn::keypad::b_pressed())
    {
        _itemMenuState = State::SELECT_ITEM;
        moveCursorItem(true);
    }
    else if (bn::keypad::a_pressed())
    {
        auto& gameState = _scene.getContext().gameState;

        if (_usageIdx == Usage::USE)
        {
            const auto dialogs = ItemActivator::use(_itemIdx, gameState, _scene.getContext().rng);
            BN_ASSERT(dialogs.size() <= _scene._dialogs.max_size(), "Too many dialogs=", dialogs.size(), " (max ",
                      _scene._dialogs.max_size(), ")");

            _scene._dialogs = dialogs;
            _scene.redrawTexts();
        }
        else if (_usageIdx == Usage::INFO)
        {
            const auto dialogs = ItemActivator::info(_itemIdx, gameState);
            for (const auto& dialog : dialogs)
                _scene._dialogs.push_back(dialog);
        }
        else if (_usageIdx == Usage::DROP)
        {
            _scene._dialogs.push_back(ItemActivator::drop(_itemIdx, gameState));
        }

        return MenuStateType::DIALOG;
    }
    else if (bn::keypad::left_pressed() && _usageIdx > Usage::USE)
    {
        _usageIdx -= 1;
        moveCursorUsage(true);
    }
    else if (bn::keypad::right_pressed() && _usageIdx < Usage::DROP)
    {
        _usageIdx += 1;
        moveCursorUsage(true);
    }

    return MenuStateType::NONE;
}

void ItemMenu::moveCursorItem(bool playSfx)
{
    if (playSfx)
        asset::getSfx(asset::SfxKind::MENU_CURSOR)->play();

    _scene._cursor.set_position(FIRST_ITEM_CURSOR_POS + ITEM_MARGIN * _itemIdx);
}

void ItemMenu::moveCursorUsage(bool playSfx)
{
    if (playSfx)
        asset::getSfx(asset::SfxKind::MENU_CURSOR)->play();

    _scene._cursor.set_position(USAGE_CURSOR_POSS[_usageIdx]);
}

} // namespace ut::game::menu
