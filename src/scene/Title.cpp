#include "scene/Title.hpp"

#include <bn_display.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>

#include "asset/TextColor.hpp"
#include "core/TextGens.hpp"

#include "bn_regular_bg_items_bg_startmenu.h"

namespace ut::scene
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = bn::fixed_point{-bn::display::width() / 2, -bn::display::height() / 2};

constexpr auto NAME_POS = bn::fixed_point{38, 52} + TOP_LEFT_ORIGIN;
constexpr auto TIME_POS = bn::fixed_point{203, 52} + TOP_LEFT_ORIGIN;
constexpr auto LV_POS = (NAME_POS + TIME_POS) / 2;
constexpr auto ROOM_POS = bn::fixed_point{38, 68} + TOP_LEFT_ORIGIN;
constexpr auto CONTINUE_POS = bn::fixed_point{55, 92} + TOP_LEFT_ORIGIN;
constexpr auto RESET_POS = bn::fixed_point{187, 92} + TOP_LEFT_ORIGIN;

} // namespace

Title::Title(SceneStack& sceneStack, Context& context)
    : Scene(sceneStack, context), _bg(bn::regular_bg_items::bg_startmenu.create_bg(0, 0))
{
    auto& textGen = getContext().textGens.get(asset::FontKind::MAIN);
    const auto prevAlign = textGen.alignment();
    const auto prevColor = textGen.palette_item();

    // TODO: Load save info from SRAM
    textGen.generate(NAME_POS, "WWWWWW", _saveInfoTexts);
    textGen.generate(ROOM_POS, "Ruins - Entrance", _saveInfoTexts);
    textGen.set_center_alignment();
    textGen.generate(LV_POS, "LV 9999", _saveInfoTexts);
    textGen.set_right_alignment();
    textGen.generate(TIME_POS, "9999:99", _saveInfoTexts);

    textGen.set_right_alignment();
    textGen.generate(RESET_POS, "Reset", _resetBtnText);
    textGen.set_left_alignment();
    textGen.set_palette_item(asset::getTextColor(asset::TextColorKind::YELLOW));
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
            reqStackClear();
            reqStackPush(SceneId::GAME);
        }
        else // reset selected
        {
            reqStackClear();
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
