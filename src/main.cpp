#include <bn_core.h>

#include "core/Random.hpp"
#include "core/TextGens.hpp"
#include "game/GameState.hpp"
#include "scene/SceneStack.hpp"

#include "scene/InputName.hpp"
#include "scene/IntroLogo.hpp"
#include "scene/IntroStory.hpp"
#include "scene/NewGameTitle.hpp"
#include "scene/Title.hpp"

#ifdef UT_TEST
#include "scene/test/SaveTest.hpp"
#endif

using namespace ut;

void registerScenes(scene::SceneStack& sceneStack);

int main()
{
    bn::core::init();
    bn::core::set_skip_frames(1); // 30 fps

    ut::core::TextGens textGens;
    ut::core::Random rng;
    ut::game::GameState gameState;
    scene::Scene::Context sceneContext(textGens, rng, gameState);

    scene::SceneStack sceneStack(sceneContext);
    registerScenes(sceneStack);

#ifndef UT_TEST
    sceneStack.pushScene(scene::SceneId::INTRO_STORY);
#else
    sceneStack.pushScene(scene::SceneId::SAVE_TEST);
#endif

    while (true)
    {
        sceneStack.handleInput();
        sceneStack.update();

        rng.update();
        bn::core::update();
    }
}

void registerScenes(scene::SceneStack& sceneStack)
{
    using namespace scene;

    sceneStack.registerScene<IntroStory>(SceneId::INTRO_STORY);
    sceneStack.registerScene<IntroLogo>(SceneId::INTRO_LOGO);
    sceneStack.registerScene<Title>(SceneId::TITLE);
    sceneStack.registerScene<NewGameTitle>(SceneId::NEW_GAME_TITLE);
    sceneStack.registerScene<InputName>(SceneId::INPUT_NAME);

#ifdef UT_TEST
    sceneStack.registerScene<test::SaveTest>(SceneId::SAVE_TEST);
#endif
}
