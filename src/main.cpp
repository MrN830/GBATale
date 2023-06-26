#include <bn_core.h>

#include "core/Random.hpp"
#include "core/TextGens.hpp"
#include "game/GameState.hpp"
#include "scene/SceneStack.hpp"

#include "scene/ConfirmName.hpp"
#include "scene/Game.hpp"
#include "scene/IngameMenu.hpp"
#include "scene/InputName.hpp"
#include "scene/IntroLogo.hpp"
#include "scene/IntroStory.hpp"
#include "scene/NewGameTitle.hpp"
#include "scene/Title.hpp"

#include "config.hpp"

#if UT_TEST_SCENE
#include "scene/test/SaveTest.hpp"
#include "scene/test/WorldBgTest.hpp"
#endif
#if UT_MEM_VIEW
#include "debug/MemView.hpp"
#endif

using namespace ut;

namespace ut::asset
{
void updateDelayedSfxs();
}

void registerScenes(scene::SceneStack& sceneStack);

int main()
{
    bn::core::init();
    bn::core::set_skip_frames(1); // 30 fps

    ut::core::TextGens textGens;
    ut::core::Random rng;
    ut::game::GameState gameState;

#if UT_MEM_VIEW
    // Press `SELECT` to see the `memView` when it's enabled
    debug::MemView memView(textGens);
#endif

    scene::Scene::Context sceneContext(textGens, rng, gameState);
    scene::SceneStack sceneStack(sceneContext);
    registerScenes(sceneStack);

    sceneStack.pushScene(scene::SceneId::INTRO_STORY);

    while (true)
    {
        sceneStack.handleInput();
        sceneStack.update();

#if UT_MEM_VIEW
        memView.update();
#endif
        rng.update();
        asset::updateDelayedSfxs();
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
    sceneStack.registerScene<ConfirmName>(SceneId::CONFIRM_NAME);
    sceneStack.registerScene<Game>(SceneId::GAME);
    sceneStack.registerScene<IngameMenu>(SceneId::INGAME_MENU);

#if UT_TEST_SCENE
    sceneStack.registerScene<test::SaveTest>(SceneId::SAVE_TEST);
    sceneStack.registerScene<test::WorldBgTest>(SceneId::WORLD_BG_TEST);
#endif
}
