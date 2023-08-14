#include "scene/ConfirmName.hpp"

#include <bn_blending.h>
#include <bn_display.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_sound.h>
#include <bn_sound_item.h>

#include "asset/Bgm.hpp"
#include "asset/SfxKind.hpp"
#include "asset/TextColor.hpp"
#include "core/Dialog.hpp"
#include "core/DialogWriter.hpp"
#include "core/TextGens.hpp"
#include "game/GameState.hpp"
#include "util/String.hpp"

namespace ut::scene
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = bn::fixed_point{-bn::display::width() / 2, -bn::display::height() / 2};

constexpr auto NAME_POS = bn::fixed_point{96, 34} + TOP_LEFT_ORIGIN;

constexpr auto NO_POS = bn::fixed_point{50, 146} + TOP_LEFT_ORIGIN;
constexpr auto YES_POS = bn::fixed_point{191, 146} + TOP_LEFT_ORIGIN;

constexpr bn::fixed MOVE_DIFF_Y = 70;

constexpr int FPS = 30;
constexpr int NAME_ACTION_FRAMES = 3 * FPS;
constexpr int WHITE_OUT_FRAMES = 5 * FPS;
constexpr int GAME_SCENE_FRAMES = 5.5 * FPS;

} // namespace

ConfirmName::ConfirmName(SceneStack& sceneStack, SceneContext& context)
    : Scene(sceneStack, context, SceneId::CONFIRM_NAME), _whiteOut(WHITE_OUT_FRAMES, 1)
{
    auto& textGen = context.textGens.get(asset::FontKind::MAIN);
    const auto prevAlign = textGen.alignment();
    const auto prevColor = textGen.palette_item();

    constexpr int NAME_MAX_LEN = game::GameState::CHAR_NAME_MAX_LEN;
    bn::string<NAME_MAX_LEN> charName = context.gameState.getCharName();
    auto lowerName = util::toLowerAscii(charName);
    const bn::string_view name = lowerName;

    _isAllowed = true;
    bn::string_view tip = "Is this name correct?";

    bool isChosenName = false;

    if (context.gameState.getRSavedCount() >= 1)
    {
        // check if save file charName is `Frisk`
        game::GameState state(context.rng);
        state.loadFromRegularSave();

        if ("frisk" != bn::string_view(util::toLowerAscii(bn::string<NAME_MAX_LEN>(state.getCharName()))))
            isChosenName = true;
    }

    if (isChosenName)
        tip = "A name has already#been chosen.";
    else if (name == "gaster")
        tip = "";
    else if (name == "frisk")
        tip = "WARNING: This name will#make your life hell.#Proceed anyway?";
    else if (name == "aaaaaa")
        tip = "Not very creative...?";
    else if (name == "asgore")
        tip = "You cannot.", _isAllowed = false;
    else if (name == "toriel")
        tip = "I think you should#think of your own#name, my child.", _isAllowed = false;
    else if (name == "sans")
        tip = "nope.", _isAllowed = false;
    else if (name == "undyne")
        tip = "Get your OWN name!", _isAllowed = false;
    else if (name == "flowey")
        tip = "I already CHOSE#that name.", _isAllowed = false;
    else if (name == "chara")
        tip = "The true name.";
    else if (name == "alphys")
        tip = "D-don't do that.", _isAllowed = false;
    else if (name == "alphy")
        tip = "Uh... OK?";
    else if (name == "papyru")
        tip = "I'LL ALLOW IT!!!!";
    else if (name == "napsta" || name == "blooky")
        tip = "...........#(They're powerless to#stop you.)";
    else if (name == "murder" || name == "mercy")
        tip = "That's a little on-#the nose, isn't it...?";
    else if (name == "asriel")
        tip = "...", _isAllowed = false;
    else if (name == "catty")
        tip = "Bratty! Bratty!#That's MY name!";
    else if (name == "bratty")
        tip = "Like, OK I guess.";
    else if (name == "mtt" || name == "metta" || name == "mett")
        tip = "OOOOH!!! ARE YOU#PROMOTING MY BRAND?";
    else if (name == "gerson")
        tip = "Wah ha ha! Why not?";
    else if (name == "shyren")
        tip = "...?";
    else if (name == "aaron")
        tip = "Is this name correct? ; )";
    else if (name == "temmie")
        tip = "hOI!";
    else if (name == "woshua")
        tip = "Clean name.";
    else if (name == "jerry")
        tip = "Jerry.";
    else if (name == "bpants")
        tip = "You are really scraping the#bottom of the barrel.";

    core::Dialog dialog[] = {{core::Dialog::Settings::Kind::CONFIRM_NAME, tip}};

    core::DialogWriter dialogWriter(context.textGens);
    dialogWriter.start(dialog, _tip);
    dialogWriter.instantWrite();

    if (name == "gaster")
    {
        reqStackClear();
        reqStackPush(SceneId::INTRO_STORY);
    }
    else
    {
        textGen.generate(NAME_POS, charName, _nameSpr);
        const auto textCenterX = NAME_POS.x() + textGen.width(charName) / 2;

        for (int i = 0; i < _nameSpr.size(); ++i)
        {
            auto& spr = _nameSpr[i];

            spr.set_blending_enabled(true);

            // Scale text from the 'real' center origin
            const auto centerDiffX = spr.x() - textCenterX;
            const bn::fixed_point finalPos(spr.x() + centerDiffX, spr.y() + MOVE_DIFF_Y);

            _nameMoveAction.emplace_back(spr, NAME_ACTION_FRAMES, finalPos);
            _nameScaleAction.emplace_back(spr, NAME_ACTION_FRAMES, 2);
        }

        textGen.set_palette_item(asset::getTextColor(asset::TextColorKind::YELLOW));
        textGen.generate(NO_POS, _isAllowed ? "No" : "Go back", _no);

        if (_isAllowed)
        {
            textGen.set_right_alignment();
            textGen.set_palette_item(asset::getTextColor(asset::TextColorKind::WHITE));
            textGen.generate(YES_POS, "Yes", _yes);
        }
    }

    textGen.set_alignment(prevAlign);
    textGen.set_palette_item(prevColor);
}

ConfirmName::~ConfirmName()
{
    bn::sound::stop_all();
    bn::blending::set_fade_color(bn::blending::fade_color_type::BLACK);
    bn::blending::set_fade_alpha(0);
}

bool ConfirmName::handleInput()
{
    if (isConfirmed())
        return true;

    if (_isAllowed && (bn::keypad::left_pressed() || bn::keypad::right_pressed()))
    {
        _isYesSelected = !_isYesSelected;

        const auto& white = asset::getTextColor(asset::TextColorKind::WHITE);
        const auto& yellow = asset::getTextColor(asset::TextColorKind::YELLOW);

        for (auto& spr : _no)
            spr.set_palette(_isYesSelected ? white : yellow);
        for (auto& spr : _yes)
            spr.set_palette(_isYesSelected ? yellow : white);
    }

    if (bn::keypad::a_pressed())
    {
        // If `YES` pressed
        if (_isYesSelected)
        {
            // Go to `scene::Game` with white-out transition
            getContext().gameState.resetToNewRegularSave(getContext().rng);
            _tip.clear();
            _no.clear();
            _yes.clear();
            _gameSceneCountdown = GAME_SCENE_FRAMES;
            bn::blending::set_fade_color(bn::blending::fade_color_type::WHITE);

            asset::Bgm::stop();
            asset::getSfx(asset::SfxKind::CYMBAL_RISER)->play(3.0 / 4);
        }
        else
        {
            reqStackClear();

            if (getContext().gameState.getRSavedCount() >= 1)
                reqStackPush(SceneId::TITLE);
            else
                reqStackPush(SceneId::INPUT_NAME);
        }
    }

    return true;
}

bool ConfirmName::update()
{
    for (auto& action : _nameMoveAction)
        if (!action.done())
            action.update();
    for (auto& action : _nameScaleAction)
        if (!action.done())
            action.update();

    if (isConfirmed())
    {
        if (!_whiteOut.done())
            _whiteOut.update();

        if (--_gameSceneCountdown <= 0)
        {
            reqStackClear();
            reqStackPush(SceneId::GAME);
        }
    }

    return true;
}

bool ConfirmName::isConfirmed() const
{
    return _gameSceneCountdown >= 0;
}

} // namespace ut::scene
