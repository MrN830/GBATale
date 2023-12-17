#include "game/cpnt/ev/TimedDestroy.hpp"

#include "game/ent/Entity.hpp"
#include "game/task/TaskAwaiters.hpp"

namespace ut::game::cpnt::ev
{

TimedDestroy::TimedDestroy(ent::Entity& entity, bool isEnabled, bool isAutoFire)
    : EventComponent(entity, bn::type_id<TimedDestroy>(), isEnabled, isAutoFire)
{
}

auto TimedDestroy::onEvent(GameContext&) -> task::Task
{
    if (_hasFired)
        co_return;

    _hasFired = true;

    co_await task::TimeAwaiter(_destroyTicks);

    _entity.setDestroyed(true);
    co_return;
}

void TimedDestroy::setDestroyTicks(int ticks)
{
    _destroyTicks = ticks;
}

} // namespace ut::game::cpnt::ev
