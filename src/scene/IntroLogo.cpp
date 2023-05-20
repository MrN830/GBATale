#include "scene/IntroLogo.hpp"

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
}

bool IntroLogo::handleInput()
{
    return true;
}

bool IntroLogo::update()
{
    ++_elapsedFrames;

    if (_elapsedFrames >= RESTART_INTRO_STORY_FRAMES)
    {
        reqStackClear();
        reqStackPush(SceneId::INTRO_STORY);
    }

    return true;
}

} // namespace ut::scene
