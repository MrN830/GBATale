#include "game/ent/Entity.hpp"

#include <bn_camera_ptr.h>

#include "game/GameContext.hpp"
#include "game/mngr/EntityManager.hpp"

namespace ut::game::ent
{

void Entity::handleInput(GameContext& context)
{
    for (auto& cpnt : _components)
        if (cpnt.isEnabled())
            cpnt.handleInput(context);
}

void Entity::update(GameContext& context)
{
    for (auto& cpnt : _components)
        if (cpnt.isEnabled())
            cpnt.update(context);
}

void Entity::render(GameContext& context)
{
    for (auto& cpnt : _components)
        if (cpnt.isEnabled())
            cpnt.render(context);
}

bool Entity::isActive() const
{
    return _isActive;
}

bool Entity::isDestroyed() const
{
    return _isDestroyed;
}

auto Entity::getPosition() const -> const bn::fixed_point&
{
    return _position;
}

void Entity::setActive(bool isActive)
{
    if (_isActive == isActive)
        return;

    _isActive = isActive;

    for (auto& cpnt : _components)
        cpnt.onEntityActiveChanged(isActive);
}

void Entity::setDestroyed(bool isDestroyed)
{
    _isDestroyed = isDestroyed;
}

void Entity::setPosition(const bn::fixed_point& position)
{
    _position = position;
}

} // namespace ut::game::ent
