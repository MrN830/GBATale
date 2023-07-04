#include "game/mngr/EntityManager.hpp"

#include "asset/SpriteAnimKind.hpp"
#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/cpnt/PlayerInput.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/ent/Entity.hpp"

#if UT_MEM_VIEW
#include "debug/MemView.hpp"
#endif

#include "bn_sprite_items_ch_frisk_base.h"

namespace ut::game::mngr
{

EntityManager::EntityManager() : _cpntHeap(_cpntBuffer, sizeof(_cpntBuffer))
{
#if UT_MEM_VIEW
    debug::MemView::instance().setEntMngr(this);
#endif
}

EntityManager::~EntityManager()
{
    removeDestroyed(true);

#if UT_MEM_VIEW
    debug::MemView::instance().setEntMngr(nullptr);
#endif
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

void EntityManager::createFrisk(const bn::fixed_point position, const GameContext& context)
{
    ent::Entity& frisk = _entPool.create();
    _entities.push_front(frisk);

    frisk.setPosition(position);

    cpnt::Sprite& spr =
        _cpntHeap.create<cpnt::Sprite>(frisk, bn::sprite_items::ch_frisk_base, 1, &context.camera, true);
    frisk.addComponent(spr);

    using AnimKind = asset::SpriteAnimKind;
    cpnt::SpriteAnim& sprAnim = _cpntHeap.create<cpnt::SpriteAnim>(frisk, spr);
    frisk.addComponent(sprAnim);
    sprAnim.registerDirectionAnimKinds(AnimKind::FRISK_WALK_UP, AnimKind::FRISK_WALK_DOWN, AnimKind::FRISK_WALK_LEFT,
                                       AnimKind::FRISK_WALK_RIGHT);

    cpnt::PlayerInput& input = _cpntHeap.create<cpnt::PlayerInput>(frisk);
    frisk.addComponent(input);
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
