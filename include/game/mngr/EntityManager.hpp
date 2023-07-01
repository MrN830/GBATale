#include <bn_best_fit_allocator.h>
#include <bn_intrusive_forward_list.h>
#include <bn_pool.h>

#include "game/ent/Entity.hpp"

namespace ut::game
{
struct GameContext;
}

namespace ut::game::mngr
{

class EntityManager final
{
public:
    EntityManager();
    ~EntityManager();

    void handleInput(GameContext&);
    void update(GameContext&);

public:
    void reloadRoom(const GameContext&);

private:
    void removeDestroyed(bool forceRemoveAll);

private:
    bn::pool<ent::Entity, 32> _entPool;
    bn::intrusive_forward_list<ent::Entity> _entities;

    uint8_t _cpntBuffer[1024];
    bn::best_fit_allocator _cpntHeap;
};

} // namespace ut::game::mngr
