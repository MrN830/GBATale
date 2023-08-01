#include "game/cpnt/ev/EventComponent.hpp"

namespace ut::game::cpnt::ev
{

EventComponent::EventComponent(ent::Entity& entity, bn::type_id_t eventComponentType, bool isEnabled, bool isAutoFire)
    : Component(entity, bn::type_id<EventComponent>(), isEnabled), _isAutoFire(isAutoFire),
      _eventComponentType(eventComponentType)
{
}

auto EventComponent::getEventComponentType() const -> bn::type_id_t
{
    return _eventComponentType;
}

void EventComponent::update(GameContext& ctx)
{
    if (_isAutoFire && !_hasAutoFired)
    {
        onEvent(ctx);
        _hasAutoFired = true;
    }
}

} // namespace ut::game::cpnt::ev
