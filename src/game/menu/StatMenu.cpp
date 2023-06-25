#include "game/menu/StatMenu.hpp"

#include <bn_display.h>
#include <bn_format.h>
#include <bn_keypad.h>
#include <bn_sound_item.h>

#include "asset/FontKind.hpp"
#include "asset/SfxKind.hpp"
#include "core/TextGens.hpp"
#include "game/GameState.hpp"
#include "game/ItemInfo.hpp"
#include "game/StatInfo.hpp"
#include "game/menu/MenuStateType.hpp"
#include "scene/IngameMenu.hpp"

#include "bn_regular_bg_items_bg_ingame_menu_stat1.h"
#include "bn_regular_bg_items_bg_ingame_menu_stat2.h"

namespace ut::game::menu
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = -bn::fixed_point{bn::display::width() / 2, bn::display::height() / 2};

constexpr bn::fixed LEFT_X = 80, RIGHT_X = 150;
constexpr bn::fixed MARGIN_Y = 13;

constexpr auto NAME_POS = bn::fixed_point{LEFT_X, 16} + TOP_LEFT_ORIGIN;

constexpr bn::array<bn::fixed_point, 3> HACKED_NAME_POSS = {
    bn::fixed_point{RIGHT_X + TOP_LEFT_ORIGIN.x(), NAME_POS.y()},
    bn::fixed_point{RIGHT_X + TOP_LEFT_ORIGIN.x(), NAME_POS.y() + MARGIN_Y * 1},
    bn::fixed_point{RIGHT_X + TOP_LEFT_ORIGIN.x(), NAME_POS.y() + MARGIN_Y * 2},
};

constexpr auto LV_POS = bn::fixed_point{LEFT_X, 36} + TOP_LEFT_ORIGIN;
constexpr auto HP_POS = bn::fixed_point{LEFT_X, 36 + MARGIN_Y} + TOP_LEFT_ORIGIN;
constexpr auto AT_POS = bn::fixed_point{LEFT_X, 70} + TOP_LEFT_ORIGIN;
constexpr auto DF_POS = bn::fixed_point{LEFT_X, 70 + MARGIN_Y} + TOP_LEFT_ORIGIN;
constexpr auto EXP_POS = bn::fixed_point{RIGHT_X + TOP_LEFT_ORIGIN.x(), AT_POS.y()};
constexpr auto NEXT_POS = bn::fixed_point{RIGHT_X + TOP_LEFT_ORIGIN.x(), DF_POS.y()};
constexpr auto WEAPON_POS = bn::fixed_point{LEFT_X, 110} + TOP_LEFT_ORIGIN;
constexpr auto ARMOR_POS = bn::fixed_point{LEFT_X, 110 + MARGIN_Y} + TOP_LEFT_ORIGIN;
constexpr auto GOLD_POS = bn::fixed_point{LEFT_X, 140} + TOP_LEFT_ORIGIN;
constexpr auto KILLS_POS = bn::fixed_point{RIGHT_X + TOP_LEFT_ORIGIN.x(), GOLD_POS.y()};

} // namespace

StatMenu::~StatMenu()
{
    _scene._cursor.set_visible(true);
}

StatMenu::StatMenu(scene::IngameMenu& scene) : MenuState(scene)
{
    scene._bg.set_item(scene.isDialogUpper() ? bn::regular_bg_items::bg_ingame_menu_stat2
                                             : bn::regular_bg_items::bg_ingame_menu_stat1);
    scene._cursor.set_visible(false);

    auto& textGen = scene.getContext().textGens.get(asset::FontKind::MAIN);

    const auto& state = scene.getContext().gameState;
    const auto& weapon = ItemInfo::get(state.getWeapon());
    const auto& armor = ItemInfo::get(state.getArmor());

    constexpr int NAME_MAX_LEN = game::GameState::CHAR_NAME_MAX_LEN;

    const auto charName = state.getCharName();
    textGen.generate(NAME_POS, bn::format<NAME_MAX_LEN + 2>("\"{}\"", charName), _text);
    // too long name, hacked name detected
    if (charName.size() > 6)
    {
        textGen.generate(HACKED_NAME_POSS[0], "Hard to", _text);
        textGen.generate(HACKED_NAME_POSS[1], "change,", _text);
        textGen.generate(HACKED_NAME_POSS[2], "huh?", _text);
    }

    textGen.generate(LV_POS, bn::format<14>("LV {}", state.getLv()), _text);
    textGen.generate(HP_POS, bn::format<26>("HP {}/{}", state.getCurHp(), state.getMaxHp()), _text);
    textGen.generate(AT_POS, bn::format<27>("AT {} ({})", state.getBaseAtk() - 10, (int)weapon.atk + armor.atk), _text);
    textGen.generate(DF_POS, bn::format<27>("DF {} ({})", state.getBaseDef() - 10, (int)weapon.def + armor.def), _text);
    textGen.generate(EXP_POS, bn::format<16>("EXP: {}", state.getExp()), _text);
    textGen.generate(NEXT_POS, bn::format<17>("NEXT: {}", StatInfo::getNextLvUpExp(state.getExp())), _text);
    textGen.generate(WEAPON_POS, bn::format<19>("WEAPON: {}", weapon.getName()), _text);
    textGen.generate(ARMOR_POS, bn::format<18>("ARMOR: {}", armor.getName()), _text);
    textGen.generate(GOLD_POS, bn::format<17>("GOLD: {}", state.getGold()), _text);
    if (state.getKills() > 20)
        textGen.generate(KILLS_POS, bn::format<18>("KILLS: {}", state.getKills()), _text);

    for (auto& spr : _text)
        spr.set_bg_priority(scene::IngameMenu::BG_PRIORITY);
}

auto StatMenu::handleInput() -> MenuStateType
{
    if (bn::keypad::b_pressed())
        return MenuStateType::MAIN;

    return MenuStateType::NONE;
}

auto StatMenu::update() -> MenuStateType
{
    return MenuStateType::NONE;
}

} // namespace ut::game::menu
