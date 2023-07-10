#include "scene/Intro.hpp"

namespace ut::scene
{

Intro::Intro(SceneStack& sceneStack, SceneContext& context) : Scene(sceneStack, context)
{
}

bool Intro::handleInput()
{
    return true;
}

bool Intro::update()
{
    return true;
}

} // namespace ut::scene
