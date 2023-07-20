#include "game/sys/EntityManager.hpp"

#include <bn_camera_ptr.h>
#include <bn_display.h>
#include <bn_sprite_shape_size.h>

#include "asset/SpriteAnimKind.hpp"
#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/PlayerInput.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/sys/CameraManager.hpp"
#include "mtile/MTilemap.hpp"

#include "gen/EntityId.hpp"

#if UT_MEM_VIEW
#include "debug/MemView.hpp"
#endif

#include "bn_sprite_items_ch_frisk_base.h"

namespace ut::game::sys
{

EntityManager::EntityManager(GameContext& context)
    : _context(context), _cpntHeap(_cpntBuffer, sizeof(_cpntBuffer)), _friskAnimDirection(core::Directions::DOWN)
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

void EntityManager::handleInput()
{
    for (auto& entity : _entities)
        if (entity.isActive())
            entity.handleInput(_context);
}

void EntityManager::update()
{
    for (auto& entity : _entities)
        if (entity.isActive())
            entity.update(_context);

    for (auto& entity : _entities)
        if (entity.isActive())
            entity.render(_context);

    removeDestroyed(false);
}

void EntityManager::reloadRoom(GameContext& ctx)
{
    removeDestroyed(true);

    const auto room = _context.state.getRoom();
    const auto mTilemap = game::getRoomMTilemap(room);
    BN_ASSERT(mTilemap != nullptr, "Invalid room=", (int)room);

    const bn::fixed_point* friskPos = mTilemap->getWarpPoint(_context.warpId);
    BN_ASSERT(friskPos != nullptr, "Invalid Frisk warp point=", (int)_context.warpId, " in room=", (int)room);

    // Load entities from new room
    createFrisk(*friskPos);
    for (const auto& entInfo : mTilemap->getEntities())
        entInfo.create(ctx);
}

void EntityManager::createFrisk(const bn::fixed_point position)
{
    ent::Entity& frisk = _entPool.create(ent::gen::EntityId::frisk, position);
    _entities.push_front(frisk);

    _context.camMngr.setCamFollowEntity(&frisk);

    cpnt::Sprite& spr = _cpntHeap.create<cpnt::Sprite>(frisk, true, bn::sprite_items::ch_frisk_base, 1,
                                                       &_context.camMngr.getCamera(), true);
    frisk.addComponent(spr);

    cpnt::SpriteAnim& sprAnim = _cpntHeap.create<cpnt::SpriteAnim>(frisk, true, spr);
    frisk.addComponent(sprAnim);

    cpnt::WalkAnimCtrl& walkAnimCtrl = _cpntHeap.create<cpnt::WalkAnimCtrl>(frisk, true, sprAnim);
    frisk.addComponent(walkAnimCtrl);
    walkAnimCtrl.registerWalkAnimKind(asset::WalkAnimKind::FRISK);
    walkAnimCtrl.setStandStillDir(_friskAnimDirection);

    cpnt::PlayerInput& input = _cpntHeap.create<cpnt::PlayerInput>(frisk, true, walkAnimCtrl);
    frisk.addComponent(input);

    cpnt::ColliderPack& collPack = _cpntHeap.create<cpnt::ColliderPack>(frisk, true, false);
    frisk.addComponent(collPack);
    const auto& sprSize = bn::sprite_items::ch_frisk_base.shape_size();
    const bn::fixed_size collSize = {16, 9};
    const bn::fixed_point collPos = {0 + collSize.width() / 2 - sprSize.width() / 2,
                                     23 + collSize.height() / 2 - sprSize.height()};
    coll::Collider& coll = _collPool.create(coll::CollInfo(coll::RectCollInfo(collPos, collSize)));
    collPack.addDynamicCollider(coll);
}

auto EntityManager::findById(ent::gen::EntityId entityId) -> ent::Entity*
{
    if (entityId == ent::gen::EntityId::NONE)
        return nullptr;

    for (auto it = _entities.begin(); it != _entities.end(); ++it)
        if (it->getId() == entityId)
            return &*it;

    return nullptr;
}

auto EntityManager::findById(ent::gen::EntityId entityId) const -> const ent::Entity*
{
    if (entityId == ent::gen::EntityId::NONE)
        return nullptr;

    for (auto it = _entities.cbegin(); it != _entities.cend(); ++it)
        if (it->getId() == entityId)
            return &*it;

    return nullptr;
}

auto EntityManager::beforeBeginIter() -> decltype(_entities)::iterator
{
    return _entities.before_begin();
}

auto EntityManager::cBeforeBeginIter() const -> decltype(_entities)::const_iterator
{
    return _entities.cbefore_begin();
}

auto EntityManager::endIter() -> decltype(_entities)::iterator
{
    return _entities.end();
}

auto EntityManager::cEndIter() const -> decltype(_entities)::const_iterator
{
    return _entities.cend();
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

        // Detach camera if it was following `entity`
        auto& camMngr = _context.camMngr;
        if (&entity == camMngr.getCamFollowEntity())
        {
            camMngr.setCamFollowEntity(nullptr);
        }

        if (entity.getId() == ent::gen::EntityId::frisk)
        {
            const auto* friskWalk = entity.getComponent<cpnt::WalkAnimCtrl>();
            BN_ASSERT(friskWalk != nullptr);

            // Preserve Frisk's direction
            _friskAnimDirection = friskWalk->getLastAnimDir();
        }

        // Destroy dynamic colliders within `ColliderPack`
        auto* collPack = entity.getComponent<cpnt::ColliderPack>();
        if (collPack != nullptr)
        {
            auto& colls = collPack->_dynamicColls;
            for (auto cBeforeIt = colls.before_begin(), cIt = colls.begin(); cIt != colls.end();)
            {
                auto& coll = *cIt;
                cIt = colls.erase_after(cBeforeIt);
                _collPool.destroy(coll);
            }
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
        eIt = _entities.erase_after(eBeforeIt);
        _entPool.destroy(entity);
    }

    if (forceRemoveAll)
    {
        BN_ASSERT(_entities.empty());
        BN_ASSERT(_entPool.empty());
        BN_ASSERT(_cpntHeap.empty());
    }
}

} // namespace ut::game::sys
