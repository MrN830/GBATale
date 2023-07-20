#include "game/cpnt/Component.hpp"

namespace ut::game::cpnt
{

Component::~Component() = default;

Component::Component(ent::Entity& entity, bn::type_id_t type) : _entity(entity), _type(type)
{
}

auto Component::getType() const -> bn::type_id_t
{
    return _type;
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
