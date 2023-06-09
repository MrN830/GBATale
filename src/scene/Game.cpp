#include "scene/Game.hpp"

namespace ut::scene
{

Game::Game(SceneStack& sceneStack, Context& context) : Scene(sceneStack, context)
{
}

bool Game::handleInput()
{
    return true;
}

bool Game::update()
{
    // TEST: temporarily redirect to `WorldBgTest`
    reqStackClear();
    reqStackPush(SceneId::WORLD_BG_TEST);

    return true;
}

} // namespace ut::scene
