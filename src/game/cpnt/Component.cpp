#include "game/cpnt/Component.hpp"

namespace ut::game::cpnt
{

Component::Component(ent::Entity& entity) : _entity(entity)
{
}

void Component::onEntityActiveChanged([[maybe_unused]] bool isEntityActive)
{
}

bool Component::isEnabled() const
{
    return _isEnabled;
}

void Component::setEnabled(bool isEnabled)
{
    _isEnabled = isEnabled;
}

} // namespace ut::game::cpnt
