#include <bn_core.h>

#include "core/TextGens.hpp"
#include "scene/SceneStack.hpp"

#include "scene/IntroLogo.hpp"
#include "scene/IntroStory.hpp"
#include "scene/NewGameTitle.hpp"
#include "scene/Title.hpp"

using namespace ut;

void registerScenes(scene::SceneStack& sceneStack);

int main()
{
    bn::core::init();
    bn::core::set_skip_frames(1); // 30 fps

    ut::core::TextGens textGens;
    scene::Scene::Context sceneContext(textGens);

    scene::SceneStack sceneStack(sceneContext);
    registerScenes(sceneStack);

    sceneStack.pushScene(scene::SceneId::INTRO_STORY);

    while (true)
    {
        sceneStack.handleInput();
        sceneStack.update();

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
}
