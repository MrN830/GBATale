#pragma once

#include <bn_best_fit_allocator.h>
#include <bn_intrusive_forward_list.h>
#include <bn_pool.h>

#include "game/coll/Collider.hpp"
#include "game/ent/Entity.hpp"

#include "config.hpp"

namespace ut::game
{
struct GameContext;
}

#if UT_MEM_VIEW
namespace ut::debug
{
class MemView;
}
#endif

namespace ut::game::ent
{
struct EntityInfo;
}
namespace ut::game::ent::gen
{
enum class EntityId : uint16_t;
}

namespace ut::game::sys
{

class EntityManager final
{
#if UT_MEM_VIEW
    friend class ut::debug::MemView;
#endif

    friend class ut::game::ent::EntityInfo;

private:
    GameContext& _context;

    bn::pool<ent::Entity, 64> _entPool;
    bn::intrusive_forward_list<ent::Entity> _entities;

    alignas(4) uint8_t _cpntBuffer[8192];
    bn::best_fit_allocator _cpntHeap;

    bn::pool<coll::Collider, 64> _collPool;

    core::Directions _friskAnimDirection;

public:
    EntityManager(GameContext&);
    ~EntityManager();

    void handleInput();
    void update();

public:
    void reloadRoom(GameContext&);

    void createFrisk(const bn::fixed_point& position);
    void createStalkerFlowey(const bn::fixed_point& position);

public:
    /**
     * @brief Finds the first entity with a given `EntityId`.
     *
     * You CANNOT find entities with duplicated `EntityId`s in a same room with this method.
     * If you want that, you need to use `findIf()` instead.
     */
    auto findById(ent::gen::EntityId) -> ent::Entity*;

    /**
     * @brief Finds the first entity with a given `EntityId`.
     *
     * You CANNOT find entities with duplicated `EntityId`s in a same room with this method.
     * If you want that, you need to use `findIf()` instead.
     */
    auto findById(ent::gen::EntityId) const -> const ent::Entity*;

    template <typename Cond>
    auto findIf(Cond condition, decltype(_entities)::iterator prevIt) -> decltype(_entities)::iterator;
    template <typename Cond>
    auto findIf(Cond condition, decltype(_entities)::const_iterator prevIt) const
        -> decltype(_entities)::const_iterator;

    auto beforeBeginIter() -> decltype(_entities)::iterator;
    auto cBeforeBeginIter() const -> decltype(_entities)::const_iterator;

    auto endIter() -> decltype(_entities)::iterator;
    auto cEndIter() const -> decltype(_entities)::const_iterator;

private:
    void removeDestroyed(bool forceRemoveAll);
};

} // namespace ut::game::sys

#include "game/sys/EntityManager.inl"
