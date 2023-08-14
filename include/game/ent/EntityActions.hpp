#pragma once

#include <functional>

#include <bn_value_template_actions.h>

#include "game/ent/Entity.hpp"

namespace ut::game::ent
{

class EntityPositionManager
{
public:
    static auto get(const std::reference_wrapper<Entity> entity) -> const bn::fixed_point&
    {
        return entity.get().getPosition();
    }

    static void set(const bn::fixed_point& position, std::reference_wrapper<Entity> entity)
    {
        entity.get().setPosition(position);
    }
};

class EntityMoveToAction
    : public bn::to_value_template_action<std::reference_wrapper<Entity>, bn::fixed_point, EntityPositionManager>
{
public:
    EntityMoveToAction(Entity& entity, int durationUpdates, bn::fixed_point finalPosition)
        : to_value_template_action(entity, durationUpdates, finalPosition)
    {
    }

    EntityMoveToAction(Entity& entity, int durationUpdates, bn::fixed finalX, bn::fixed finalY)
        : to_value_template_action(entity, durationUpdates, bn::fixed_point(finalX, finalY))
    {
    }
};

} // namespace ut::game::ent
