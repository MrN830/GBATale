#include <bn_best_fit_allocator.h>
#include <bn_intrusive_forward_list.h>
#include <bn_pool.h>

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

namespace ut::game::mngr
{

class EntityManager final
{
#if UT_MEM_VIEW
    friend class ut::debug::MemView;
#endif

public:
    EntityManager();
    ~EntityManager();

    void handleInput(GameContext&);
    void update(GameContext&);

public:
    void reloadRoom(const GameContext&);

public:
    void createFrisk(const bn::fixed_point position, const GameContext&);

private:
    void removeDestroyed(bool forceRemoveAll);

private:
    bn::pool<ent::Entity, 32> _entPool;
    bn::intrusive_forward_list<ent::Entity> _entities;

    uint8_t _cpntBuffer[1024];
    bn::best_fit_allocator _cpntHeap;
};

} // namespace ut::game::mngr
