#include "game/cpnt/ev/EventComponent.hpp"

#include "game/GameContext.hpp"
#include "game/sys/TaskManager.hpp"

namespace ut::game::cpnt::ev
{

EventComponent::EventComponent(ent::Entity& entity, EventComponentType eventComponentType, bool isEnabled, bool isAutoFire)
    : Component(entity, bn::type_id<EventComponent>(), isEnabled), _isAutoFire(isAutoFire),
      _eventComponentType(eventComponentType)
{
}

auto EventComponent::getEventComponentType() const -> EventComponentType
{
    return _eventComponentType;
}

void EventComponent::update(GameContext& ctx)
{
    if (_isAutoFire && !_hasAutoFired)
    {
        _hasAutoFired = true;

        auto task = onEvent(ctx);
        if (!task.done())
            ctx.taskMngr.addTask(std::move(task));
    }
}

} // namespace ut::game::cpnt::ev
