#include "scene/Scene.hpp"

#include "scene/SceneStack.hpp"

namespace ut::scene
{

Scene::Scene(SceneStack& sceneStack, SceneContext& context, SceneId sceneId)
    : _sceneStack(sceneStack), _context(context), _sceneId(sceneId)
{
}

auto Scene::getId() const -> SceneId
{
    return _sceneId;
}

auto Scene::getSceneStack() const -> const SceneStack&
{
    return _sceneStack;
}

auto Scene::getSceneStack() -> SceneStack&
{
    return _sceneStack;
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
