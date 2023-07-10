#include "scene/Scene.hpp"

#include "scene/SceneStack.hpp"

namespace ut::scene
{

Scene::Scene(SceneStack& sceneStack, SceneContext& context) : _sceneStack(sceneStack), _context(context)
{
}

void Scene::reqStackPush(SceneId sceneId)
{
    _sceneStack.pushScene(sceneId);
}

void Scene::reqStackPop()
{
    _sceneStack.popScene();
}

void Scene::reqStackClear()
{
    _sceneStack.clearScene();
}

auto Scene::getContext() -> SceneContext&
{
    return _context;
}

auto Scene::getContext() const -> const SceneContext&
{
    return _context;
}

} // namespace ut::scene
