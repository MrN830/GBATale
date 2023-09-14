#include "scene/SavePrompt.hpp"

#include <bn_display.h>
#include <bn_fixed_point.h>
#include <bn_format.h>
#include <bn_keypad.h>
#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "asset/TextColor.hpp"
#include "core/TextGens.hpp"
#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"

#include "consts.hpp"

#include "bn_regular_bg_items_bg_save_prompt.h"
#include "bn_sprite_items_spr_soul_heart.h"

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
constexpr auto SAVE_POS = bn::fixed_point{50, 92} + TOP_LEFT_ORIGIN;
constexpr auto RETURN_POS = bn::fixed_point{137, 92} + TOP_LEFT_ORIGIN;

constexpr auto CURSOR_SAVE_POS = SAVE_POS + bn::fixed_point{-8, 0};
constexpr auto CURSOR_RETURN_POS = RETURN_POS + bn::fixed_point{-8, 0};

} // namespace

SavePrompt::SavePrompt(SceneStack& sceneStack, SceneContext& context)
    : Scene(sceneStack, context, SceneId::SAVE_PROMPT), _bg(bn::regular_bg_items::bg_save_prompt.create_bg(0, 0)),
      _cursor(bn::sprite_items::spr_soul_heart.create_sprite(CURSOR_SAVE_POS))
{
    auto* ctx = getContext().gameContext;
    BN_ASSERT(ctx != nullptr);

    _bg.set_priority(consts::INGAME_MENU_BG_PRIORITY);
    _cursor.set_bg_priority(consts::INGAME_MENU_BG_PRIORITY);

    redrawTexts();
}

SavePrompt::~SavePrompt()
{
    auto* ctx = getContext().gameContext;
    BN_ASSERT(ctx != nullptr);

    BN_ASSERT(ctx->interactStack.top() == game::InteractState::SAVE_PROMPT);
    ctx->interactStack.pop();
}

bool SavePrompt::handleInput()
{
    if (bn::keypad::a_pressed())
    {
        if (!_isSaved && _isSaveSelected)
        {
            getContext().gameState.saveRegular();
            _isSaved = true;

            asset::getSfx(asset::SfxKind::SAVE)->play();

            _cursor.set_visible(false);
            redrawTexts();
        }
        else
        {
            reqStackPop();
        }
    }
    else if (bn::keypad::b_pressed())
    {
        reqStackPop();
    }
    else if (!_isSaved && (bn::keypad::left_pressed() || bn::keypad::right_pressed()))
    {
        _isSaveSelected = !_isSaveSelected;

        asset::getSfx(asset::SfxKind::MENU_CURSOR)->play();

        _cursor.set_position(_isSaveSelected ? CURSOR_SAVE_POS : CURSOR_RETURN_POS);
    }

    return true;
}

bool SavePrompt::update()
{
    return true;
}

void SavePrompt::redrawTexts()
{
    _saveInfoTexts.clear();
    _saveBtnText.clear();
    _returnBtnText.clear();

    auto& textGen = getContext().textGens.get(asset::FontKind::MAIN);
    const auto prevAlign = textGen.alignment();
    const auto prevColor = textGen.palette_item();

    const auto& yellow = asset::getTextColor(asset::TextColorKind::YELLOW);
    if (_isSaved)
        textGen.set_palette_item(yellow);

    game::GameState state(getContext().rng);
    state.loadFromRegularSave();

    textGen.generate(NAME_POS, state.isNewRegularSave() ? "EMPTY" : state.getCharName(), _saveInfoTexts);

    const auto& roomName = game::getRoomName(state.getRoom());
    bn::fixed_point roomPos = ROOM_POS_LEFT;
    if (textGen.width(roomName) >= TIME_POS.x() - NAME_POS.x())
    {
        textGen.set_center_alignment();
        roomPos = ROOM_POS_CENTER;
    }
    textGen.generate(roomPos, roomName, _saveInfoTexts);

    textGen.set_center_alignment();
    textGen.generate(LV_POS, bn::format<14>("LV {}", state.getLv()), _saveInfoTexts);
    textGen.set_right_alignment();
    textGen.generate(TIME_POS, state.getTime().getTimestamp(), _saveInfoTexts);

    textGen.set_left_alignment();

    if (_isSaved)
    {
        textGen.generate(SAVE_POS, "File saved.", _saveBtnText);
    }
    else
    {
        textGen.generate(SAVE_POS, "Save", _saveBtnText);
        textGen.generate(RETURN_POS, "Return", _returnBtnText);
    }

    textGen.set_alignment(prevAlign);
    textGen.set_palette_item(prevColor);

    for (auto& spr : _saveInfoTexts)
        spr.set_bg_priority(consts::INGAME_MENU_BG_PRIORITY);
    for (auto& spr : _saveBtnText)
        spr.set_bg_priority(consts::INGAME_MENU_BG_PRIORITY);
    for (auto& spr : _returnBtnText)
        spr.set_bg_priority(consts::INGAME_MENU_BG_PRIORITY);
}

} // namespace ut::scene
