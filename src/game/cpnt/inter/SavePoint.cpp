#include "game/cpnt/inter/SavePoint.hpp"

#include <bn_log.h> // test

namespace ut::game::cpnt::inter
{

SavePoint::SavePoint(ent::Entity& entity, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<SavePoint>(), triggers)
{
}

void SavePoint::onInteract()
{
    Interaction::onInteract();

    BN_LOG("TODO: Interact with `SavePoint`!");
}

} // namespace ut::game::cpnt::inter
