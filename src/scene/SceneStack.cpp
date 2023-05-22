#include "scene/SceneStack.hpp"

#include <bn_assert.h>
#include <bn_format.h>

namespace ut::scene
{

SceneStack::SceneStack(Scene::Context& context) : _context(context)
{
}

void SceneStack::handleInput()
{
    for (auto it = _stack.rbegin(); it != _stack.rend(); ++it)
    {
        if (!(*it)->handleInput())
            break;
    }

    applyPendingChanges();
}

void SceneStack::update()
{
    for (auto it = _stack.rbegin(); it != _stack.rend(); ++it)
    {
        if (!(*it)->update())
            break;
    }

    applyPendingChanges();
}

void SceneStack::pushScene(SceneId sceneId)
{
    BN_ASSERT(!_pendingChanges.full(), "`SceneStack::_pendingChanges` is full");

    _pendingChanges.push_back(PendingChange(PendingChange::Kind::PUSH, sceneId));
}

void SceneStack::popScene()
{
    BN_ASSERT(!_pendingChanges.full(), "`SceneStack::_pendingChanges` is full");

    _pendingChanges.push_back(PendingChange(PendingChange::Kind::POP));
}

void SceneStack::clearScene()
{
    BN_ASSERT(!_pendingChanges.full(), "`SceneStack::_pendingChanges` is full");

    _pendingChanges.push_back(PendingChange(PendingChange::Kind::CLEAR));
}

bool SceneStack::isEmpty() const
{
    return _stack.empty();
}

auto SceneStack::createScene(SceneId sceneId) -> Scene::UPtr
{
    BN_ASSERT(sceneId != SceneId::NONE, "create scene requested for SceneId::NONE");

    auto& factory = _sceneFactories[(int)sceneId];
    BN_ASSERT(factory, bn::format<60>("create scene requested for unregistered sceneId={}", (int)sceneId));

    return factory();
}

void SceneStack::applyPendingChanges()
{
    for (const auto& pendingChange : _pendingChanges)
    {
        switch (pendingChange.kind)
        {
            using Kind = PendingChange::Kind;
        case Kind::PUSH:
            _stack.push_back(createScene(pendingChange.sceneId));
            break;
        case Kind::POP:
            BN_ASSERT(!_stack.empty(), "pop requested for empty state stack");
            _stack.pop_back();
            break;
        case Kind::CLEAR:
            _stack.clear();
            break;
        default:
            BN_ERROR(bn::format<40>("Invalid pendingChange.kind={}", (int)pendingChange.kind));
        }
    }

    _pendingChanges.clear();
}

SceneStack::PendingChange::PendingChange(Kind kind_, SceneId SceneId_) : kind(kind_), sceneId(SceneId_)
{
}

} // namespace ut::scene
