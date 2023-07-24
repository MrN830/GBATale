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

#include "bn_regular_bg_items_bg_ingame_menu.h"
#include "bn_sprite_palette_items_pal_fttiny_white.h"

namespace ut::game::menu
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = -bn::fixed_point{bn::display::width() / 2, bn::display::height() / 2};
constexpr auto LOWER_TEXT_DIFF = bn::fixed_point{0, 2};

constexpr bn::fixed LEFT_X = 82, RIGHT_X = 159;
constexpr bn::fixed MARGIN_Y = 14;

constexpr auto NAME_POS = bn::fixed_point{LEFT_X, 18} + TOP_LEFT_ORIGIN;
constexpr auto HACKED_NAME_DETECT_POS = bn::fixed_point{148, 19} + TOP_LEFT_ORIGIN;

constexpr auto LV_POS = bn::fixed_point{LEFT_X, 38} + TOP_LEFT_ORIGIN;
constexpr auto HP_POS = bn::fixed_point{LEFT_X, 38 + MARGIN_Y} + TOP_LEFT_ORIGIN;
constexpr auto AT_POS = bn::fixed_point{RIGHT_X, 38} + TOP_LEFT_ORIGIN;
constexpr auto DF_POS = bn::fixed_point{RIGHT_X, 38 + MARGIN_Y} + TOP_LEFT_ORIGIN;
constexpr auto EXP_POS = bn::fixed_point{LEFT_X, 72} + TOP_LEFT_ORIGIN;
constexpr auto NEXT_POS = bn::fixed_point{LEFT_X, 72 + MARGIN_Y} + TOP_LEFT_ORIGIN;
constexpr auto WEAPON_POS = bn::fixed_point{LEFT_X, 106} + TOP_LEFT_ORIGIN;
constexpr auto ARMOR_POS = bn::fixed_point{LEFT_X, 106 + MARGIN_Y} + TOP_LEFT_ORIGIN;
constexpr auto GOLD_POS = bn::fixed_point{LEFT_X, 140} + TOP_LEFT_ORIGIN;
constexpr auto KILLS_POS = bn::fixed_point{RIGHT_X + TOP_LEFT_ORIGIN.x(), GOLD_POS.y()};

} // namespace

StatMenu::~StatMenu()
{
    _scene._cursor.set_visible(true);
}

StatMenu::StatMenu(scene::IngameMenu& scene) : MenuState(scene)
{
    scene._bg.set_item(bn::regular_bg_items::bg_ingame_menu, scene.isDialogUpper()
                                                                 ? scene::IngameMenu::BgMapIdx::STAT_L
                                                                 : scene::IngameMenu::BgMapIdx::STAT_U);
    scene._cursor.set_visible(false);

    auto& textGen = scene.getContext().textGens.get(asset::FontKind::MAIN);

    const auto& state = scene.getContext().gameState;
    const auto& weapon = ItemInfo::get(state.getWeapon());
    const auto& armor = ItemInfo::get(state.getArmor());

    constexpr int NAME_MAX_LEN = game::GameState::CHAR_NAME_MAX_LEN;

    const auto charName = state.getCharName();
    textGen.generate(NAME_POS + lowerTextDiff(), bn::format<NAME_MAX_LEN + 2>("\"{}\"", charName), _text);
    // too long name, hacked name detected
    if (charName.size() > 6)
    {
        auto& tinyGen = scene.getContext().textGens.get(asset::FontKind::TINY);
        const auto prevPal = tinyGen.palette_item();
        tinyGen.set_palette_item(bn::sprite_palette_items::pal_fttiny_white);

        tinyGen.generate(HACKED_NAME_DETECT_POS + lowerTextDiff(), "Easy to change, huh?", _text);

        tinyGen.set_palette_item(prevPal);
    }

    textGen.generate(LV_POS + lowerTextDiff(), bn::format<14>("LV {}", state.getLv()), _text);
    textGen.generate(HP_POS + lowerTextDiff(), bn::format<26>("HP {}/{}", state.getCurHp(), state.getMaxHp()), _text);
    textGen.generate(AT_POS + lowerTextDiff(),
                     bn::format<27>("AT {} ({})", state.getBaseAtk() - 10, (int)weapon.atk + armor.atk), _text);
    textGen.generate(DF_POS + lowerTextDiff(),
                     bn::format<27>("DF {} ({})", state.getBaseDef() - 10, (int)weapon.def + armor.def), _text);
    textGen.generate(EXP_POS + lowerTextDiff(), bn::format<16>("EXP: {}", state.getExp()), _text);
    textGen.generate(NEXT_POS + lowerTextDiff(), bn::format<17>("NEXT: {}", StatInfo::getNextLvUpExp(state.getExp())),
                     _text);
    textGen.generate(WEAPON_POS + lowerTextDiff(), bn::format<28>("WEAPON: {}", weapon.getName()), _text);
    textGen.generate(ARMOR_POS + lowerTextDiff(), bn::format<27>("ARMOR: {}", armor.getName()), _text);
    textGen.generate(GOLD_POS + lowerTextDiff(), bn::format<17>("GOLD: {}", state.getGold()), _text);
    if (state.getKills() > 20)
        textGen.generate(KILLS_POS + lowerTextDiff(), bn::format<18>("KILLS: {}", state.getKills()), _text);

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

auto StatMenu::lowerTextDiff() const -> bn::fixed_point
{
    return (_scene.isDialogUpper() ? LOWER_TEXT_DIFF : bn::fixed_point(0, 0));
}

} // namespace ut::game::menu
