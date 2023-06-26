#include "scene/IngameMenu.hpp"

#include <bn_display.h>
#include <bn_format.h>
#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "asset/TextColor.hpp"
#include "core/TextGens.hpp"
#include "game/GameState.hpp"
#include "game/menu/MenuStateType.hpp"

#include "bn_regular_bg_items_bg_ingame_menu_main1.h"
#include "bn_regular_bg_items_bg_ingame_menu_main2.h"
#include "bn_sprite_items_spr_soul_heart.h"
#include "bn_sprite_palette_items_pal_black_white.h"
#include "bn_sprite_palette_items_pal_fttiny_white.h"

using namespace ut::game::menu;

namespace ut::scene
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = -bn::fixed_point{bn::display::width() / 2, bn::display::height() / 2};

constexpr auto NAME_TEXT_POS = bn::fixed_point{8, 11} + TOP_LEFT_ORIGIN;
constexpr auto LV_TEXT_POS = bn::fixed_point{25, 20} + TOP_LEFT_ORIGIN;
constexpr auto HP_TEXT_POS = bn::fixed_point{25, 27} + TOP_LEFT_ORIGIN;
constexpr auto GOLD_TEXT_POS = bn::fixed_point{25, 34} + TOP_LEFT_ORIGIN;

constexpr bn::fixed_point MENU_TEXT_POSS[3] = {
    bn::fixed_point{23, 58} + TOP_LEFT_ORIGIN,
    bn::fixed_point{23, 72} + TOP_LEFT_ORIGIN,
    bn::fixed_point{23, 86} + TOP_LEFT_ORIGIN,
};

} // namespace

IngameMenu::IngameMenu(SceneStack& sceneStack, Context& context)
    : Scene(sceneStack, context), _menuItemCount(context.gameState.getHasPhone() ? 3 : 2), _isDialogUpper(/* TODO */),
      _bg((isDialogUpper() ? bn::regular_bg_items::bg_ingame_menu_main2 : bn::regular_bg_items::bg_ingame_menu_main1)
              .create_bg(0, 0)),
      _cursor(bn::sprite_items::spr_soul_heart.create_sprite(0, 0))
{
    changeMenuState(MenuStateType::MAIN, false);

    _bg.set_priority(BG_PRIORITY);
    _cursor.set_bg_priority(BG_PRIORITY);

    redrawTexts();
}

IngameMenu::~IngameMenu()
{
    _menuState->~MenuState();
}

bool IngameMenu::handleInput()
{
    const auto nextMenu = _menuState->handleInput();

    if (nextMenu == MenuStateType::CLOSE_MENU)
        reqStackPop();
    else if (nextMenu != MenuStateType::NONE)
        changeMenuState(nextMenu);

    return false; // only control menu, not world.
}

bool IngameMenu::update()
{
    const auto nextMenu = _menuState->update();

    if (nextMenu == MenuStateType::CLOSE_MENU)
        reqStackPop();
    else if (nextMenu != MenuStateType::NONE)
        changeMenuState(nextMenu);

    return true;
}

bool IngameMenu::isItemMenuEnabled() const
{
    return !getContext().gameState.getItems().empty();
}

bool IngameMenu::isCellMenuEnabled() const
{
    return _menuItemCount == 3;
}

bool IngameMenu::isDialogUpper() const
{
    return _isDialogUpper;
}

void IngameMenu::redrawTexts()
{
    _text.clear();

    auto& context = getContext();
    auto& textGens = context.textGens;

    auto& mainGen = textGens.get(asset::FontKind::MAIN);
    auto& cryptGen = textGens.get(asset::FontKind::CRYPT);
    auto& tinyGen = textGens.get(asset::FontKind::TINY);

    const auto prevCryptPal = cryptGen.palette_item();
    const auto prevTinyPal = tinyGen.palette_item();
    cryptGen.set_palette_item(bn::sprite_palette_items::pal_black_white);
    tinyGen.set_palette_item(bn::sprite_palette_items::pal_fttiny_white);

    const auto statDiff = (isDialogUpper() ? STAT_LOWER_DIFF : ZERO_DIFF);
    const auto menuDiff = (isDialogUpper() ? MENU_LOWER_DIFF : ZERO_DIFF);

    const auto& gameState = context.gameState;
    tinyGen.generate(NAME_TEXT_POS + statDiff, gameState.getCharName(), _text);
    cryptGen.generate(LV_TEXT_POS + statDiff, bn::to_string<11>(gameState.getLv()), _text);
    cryptGen.generate(HP_TEXT_POS + statDiff, bn::format<23>("{}/{}", gameState.getCurHp(), gameState.getMaxHp()),
                      _text);
    cryptGen.generate(GOLD_TEXT_POS + statDiff, bn::to_string<11>(gameState.getGold()), _text);

    if (!isItemMenuEnabled())
        mainGen.set_palette_item(asset::getTextColor(asset::TextColorKind::GRAY));

    mainGen.generate(MENU_TEXT_POSS[0] + menuDiff, "ITEM", _text);

    if (!isItemMenuEnabled())
        mainGen.set_palette_item(asset::getTextColor(asset::TextColorKind::WHITE));

    mainGen.generate(MENU_TEXT_POSS[1] + menuDiff, "STAT", _text);

    if (_menuItemCount >= 3)
        mainGen.generate(MENU_TEXT_POSS[2] + menuDiff, "CELL", _text);

    cryptGen.set_palette_item(prevCryptPal);
    tinyGen.set_palette_item(prevTinyPal);

    for (auto& spr : _text)
        spr.set_bg_priority(BG_PRIORITY);
}

void IngameMenu::changeMenuState(MenuStateType menuType, bool hasPrevState)
{
    if (hasPrevState)
        _menuState->~MenuState();

    switch (menuType)
    {
    case MenuStateType::MAIN:
        new (_menuState) MainMenu(*this);
        break;
    case MenuStateType::DIALOG:
        new (_menuState) DialogMenu(*this);
        break;
    case MenuStateType::ITEM:
        new (_menuState) ItemMenu(*this);
        break;
    case MenuStateType::STAT:
        new (_menuState) StatMenu(*this);
        break;
    case MenuStateType::CELL:
        new (_menuState) CellMenu(*this);
        break;

    default:
        BN_ERROR("Invalid menuType=", (int)menuType);
    }
}

} // namespace ut::scene
