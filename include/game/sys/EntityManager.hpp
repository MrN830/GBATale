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

    uint8_t _cpntBuffer[4096];
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

    auto getEntities() const -> const decltype(_entities)&;

public:
    void createFrisk(const bn::fixed_point position);

private:
    void removeDestroyed(bool forceRemoveAll);
};

} // namespace ut::game::sys
