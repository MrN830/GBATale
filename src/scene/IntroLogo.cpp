#include "scene/IntroLogo.hpp"

#include <bn_keypad.h>
#include <bn_sound.h>
#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "core/TextGens.hpp"
#include "game/GameState.hpp"

#include "bn_regular_bg_items_bg_title.h"

namespace ut::scene
{

namespace
{

constexpr int FPS = 30;

constexpr int TIP_SHOWUP_FRAMES = 3 * FPS + 9;
constexpr int RESTART_INTRO_STORY_FRAMES = 20 * FPS;

} // namespace

IntroLogo::IntroLogo(SceneStack& sceneStack, Context& context)
    : Scene(sceneStack, context), _bg(bn::regular_bg_items::bg_title.create_bg(0, 0))
{
    asset::getSfx(asset::SfxKind::INTRO_IMPACT).play();
}

IntroLogo::~IntroLogo()
{
    bn::sound::stop_all();
}

bool IntroLogo::handleInput()
{
    if (bn::keypad::a_pressed())
    {
        reqStackClear();

        if (getContext().gameState.isNewRegularSave())
            reqStackPush(SceneId::NEW_GAME_TITLE);
        else
            reqStackPush(SceneId::TITLE);
    }

    return true;
}

bool IntroLogo::update()
{
    ++_elapsedFrames;

    if (_elapsedFrames == TIP_SHOWUP_FRAMES)
    {
        auto& textGen = getContext().textGens.get(asset::FontKind::CRYPT);
        const auto prevAlign = textGen.alignment();
        textGen.set_center_alignment();

        textGen.generate(0, 40, "[Press A to start]", _tipText);

        textGen.set_alignment(prevAlign);
    }

    if (_elapsedFrames >= RESTART_INTRO_STORY_FRAMES)
    {
        reqStackClear();
        reqStackPush(SceneId::INTRO_STORY);
    }

    return true;
}

} // namespace ut::scene
