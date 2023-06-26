#include "scene/NewGameTitle.hpp"

#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_music.h>
#include <bn_music_item.h>

#include "asset/MusicKind.hpp"
#include "asset/TextColor.hpp"
#include "core/TextGens.hpp"

namespace ut::scene
{

namespace
{

constexpr bn::fixed_point TEXT_START_POS = {-79, -48};
constexpr bn::fixed LINE_HEIGHT = 16;

constexpr int TEXT_LINE_COUNT = 7;
constexpr bn::string_view TEXTS[TEXT_LINE_COUNT] = {
    "--- Instruction ---",     "", "    Ⓐ       Confirm",     "    Ⓑ       Cancel",
    "Ⓢ/🄻/🅁    Menu (In-game)", "", "When HP is 0, you lose.",
};

} // namespace

NewGameTitle::NewGameTitle(SceneStack& sceneStack, Context& context) : Scene(sceneStack, context)
{
    const auto& titleMenuMusic = *asset::getMusic(asset::MusicKind::TITLE_MENU_1);
    if (!bn::music::playing() || *bn::music::playing_item() != titleMenuMusic)
        titleMenuMusic.play(1.0 / 6);

    auto& textGen = getContext().textGens.get(asset::FontKind::MAIN);
    const auto prevAlign = textGen.alignment();
    const auto prevColor = textGen.palette_item();

    textGen.set_palette_item(asset::getTextColor(asset::TextColorKind::LIGHT_GRAY));
    textGen.set_left_alignment();

    for (int i = 0; i < TEXT_LINE_COUNT; ++i)
    {
        const auto textPos = TEXT_START_POS + bn::fixed_point{0, LINE_HEIGHT * i};
        textGen.generate(textPos, TEXTS[i], _texts);
    }

    textGen.set_alignment(prevAlign);
    textGen.set_palette_item(prevColor);
}

bool NewGameTitle::handleInput()
{
    if (bn::keypad::a_pressed())
    {
        reqStackClear();
        reqStackPush(SceneId::INPUT_NAME);
    }

    return true;
}

bool NewGameTitle::update()
{
    return true;
}

} // namespace ut::scene
