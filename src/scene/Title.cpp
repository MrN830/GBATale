#include "scene/Title.hpp"

#include <bn_display.h>
#include <bn_fixed_point.h>
#include <bn_format.h>
#include <bn_keypad.h>
#include <bn_music.h>
#include <bn_music_item.h>

#include "asset/MusicKind.hpp"
#include "asset/TextColor.hpp"
#include "core/TextGens.hpp"
#include "game/GameState.hpp"
#include "util/String.hpp"

#include "bn_regular_bg_items_bg_startmenu.h"

namespace ut::scene
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = bn::fixed_point{-bn::display::width() / 2, -bn::display::height() / 2};

constexpr auto NAME_POS = bn::fixed_point{38, 52} + TOP_LEFT_ORIGIN;
constexpr auto TIME_POS = bn::fixed_point{203, 52} + TOP_LEFT_ORIGIN;
constexpr auto LV_POS = (NAME_POS + TIME_POS) / 2;
constexpr auto ROOM_POS_LEFT = bn::fixed_point{38, 68} + TOP_LEFT_ORIGIN;
constexpr auto ROOM_POS_CENTER = bn::fixed_point{120, 68} + TOP_LEFT_ORIGIN;
constexpr auto CONTINUE_POS = bn::fixed_point{55, 92} + TOP_LEFT_ORIGIN;
constexpr auto RESET_POS = bn::fixed_point{187, 92} + TOP_LEFT_ORIGIN;

} // namespace

Title::Title(SceneStack& sceneStack, Context& context)
    : Scene(sceneStack, context), _bg(bn::regular_bg_items::bg_startmenu.create_bg(0, 0)),
      _isContinueSelected(context.menuChoice != 1)
{
    // TODO: Change music considering story progression
    const auto& titleMenuMusic = *asset::getMusic(asset::MusicKind::TITLE_MENU_1);
    if (!bn::music::playing() || *bn::music::playing_item() != titleMenuMusic)
        titleMenuMusic.play();

    auto& textGen = context.textGens.get(asset::FontKind::MAIN);
    const auto prevAlign = textGen.alignment();
    const auto prevColor = textGen.palette_item();

    const auto& yellow = asset::getTextColor(asset::TextColorKind::YELLOW);
    const auto& white = asset::getTextColor(asset::TextColorKind::WHITE);

    // we have to reload `charName` from SRAM, because it can be modified on `InputName`,
    // but it's not the real saved `charName`.
    game::GameState state;
    state.loadFromRegularSave();

    textGen.generate(NAME_POS, state.getCharName(), _saveInfoTexts);

    const bn::string_view ROOM_NAME = "Ruins - Entrance";
    bn::fixed_point roomPos = ROOM_POS_LEFT;
    if (textGen.width(ROOM_NAME) >= TIME_POS.x() - NAME_POS.x())
    {
        textGen.set_center_alignment();
        roomPos = ROOM_POS_CENTER;
    }
    textGen.generate(roomPos, ROOM_NAME, _saveInfoTexts);

    textGen.set_center_alignment();
    textGen.generate(LV_POS, bn::format<7>("LV {}", state.getLv()), _saveInfoTexts);
    textGen.set_right_alignment();
    textGen.generate(TIME_POS, state.getTime().getTimestamp(), _saveInfoTexts);

    textGen.set_right_alignment();
    textGen.set_palette_item(context.menuChoice == 1 ? yellow : white);
    textGen.generate(RESET_POS, "Reset", _resetBtnText);
    textGen.set_left_alignment();
    textGen.set_palette_item(context.menuChoice == 1 ? white : yellow);
    textGen.generate(CONTINUE_POS, "Continue", _continueBtnText);

    textGen.set_alignment(prevAlign);
    textGen.set_palette_item(prevColor);
}

bool Title::handleInput()
{
    // button activate
    if (bn::keypad::a_pressed())
    {
        if (_isContinueSelected)
        {
            getContext().menuChoice = 0;

            reqStackClear();
            reqStackPush(SceneId::GAME);
        }
        else // reset selected
        {
            getContext().menuChoice = 1;

            // we have to reload `charName` from SRAM, because it can be modified on `InputName`,
            // but it's not the real saved `charName`.
            game::GameState state;
            state.loadFromRegularSave();

            reqStackClear();

            if ("frisk" == bn::string_view(util::toLowerAscii(bn::string<8>(state.getCharName()))))
                reqStackPush(SceneId::INPUT_NAME);
            else
                reqStackPush(SceneId::CONFIRM_NAME);
        }
    }
    // button select
    else if (bn::keypad::left_pressed() || bn::keypad::right_pressed())
    {
        _isContinueSelected = !_isContinueSelected;

        if (_isContinueSelected)
        {
            for (auto& text : _continueBtnText)
                text.set_palette(asset::getTextColor(asset::TextColorKind::YELLOW));
            for (auto& text : _resetBtnText)
                text.set_palette(asset::getTextColor(asset::TextColorKind::WHITE));
        }
        else // reset selected
        {
            for (auto& text : _resetBtnText)
                text.set_palette(asset::getTextColor(asset::TextColorKind::YELLOW));
            for (auto& text : _continueBtnText)
                text.set_palette(asset::getTextColor(asset::TextColorKind::WHITE));
        }
    }

    return true;
}

bool Title::update()
{
    return true;
}

} // namespace ut::scene
