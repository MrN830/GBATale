#include "game/mngr/EntityManager.hpp"

#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/ent/Entity.hpp"

namespace ut::game::mngr
{

EntityManager::EntityManager()
{
    _cpntHeap.reset(_cpntBuffer, sizeof(_cpntBuffer));
}

EntityManager::~EntityManager()
{
    removeDestroyed(true);
}

void EntityManager::handleInput(GameContext& context)
{
    for (auto& entity : _entities)
        if (entity.isActive())
            entity.handleInput(context);
}

void EntityManager::update(GameContext& context)
{
    for (auto& entity : _entities)
        if (entity.isActive())
            entity.update(context);

    for (auto& entity : _entities)
        if (entity.isActive())
            entity.render(context);

    removeDestroyed(false);
}

void EntityManager::reloadRoom(const GameContext& context)
{
    removeDestroyed(true);

    const auto room = context.state.getRoom();
    const auto mTilemap = game::getRoomMTilemap(room);

    BN_ASSERT(mTilemap != nullptr, "Invalid room=", (int)room);

    // TODO: Load entities from new room
}

void EntityManager::removeDestroyed(bool forceRemoveAll)
{
    for (auto eBeforeIt = _entities.before_begin(), eIt = _entities.begin(); eIt != _entities.end();)
    {
        auto& entity = *eIt;
        auto& components = entity._components;

        // Do not destroy `entity` when destroy isn't requested
        if (!forceRemoveAll && !entity.isDestroyed())
        {
            eBeforeIt = eIt;
            ++eIt;
            continue;
        }

        // Destroy all components within `entity`
        for (auto cBeforeIt = components.before_begin(), cIt = components.begin(); cIt != components.end();)
        {
            auto& cpnt = *cIt;
            cIt = components.erase_after(cBeforeIt);
            _cpntHeap.destroy(cpnt);
        }

        if (forceRemoveAll)
            BN_ASSERT(components.empty());

        // Destroy `entity`
        _entities.erase_after(eBeforeIt);
        _entPool.destroy(entity);
    }

    if (forceRemoveAll)
    {
        BN_ASSERT(_entities.empty());
        BN_ASSERT(_entPool.empty());
        BN_ASSERT(_cpntHeap.empty());
    }
}

} // namespace ut::game::mngr
