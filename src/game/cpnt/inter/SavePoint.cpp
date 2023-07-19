#include "game/cpnt/inter/SavePoint.hpp"

#include <bn_log.h> // test

namespace ut::game::cpnt::inter
{

SavePoint::SavePoint(ent::Entity& entity, InteractionTriggers triggers) : Interaction(entity, triggers)
{
}

auto SavePoint::getInteractionType() const -> bn::type_id_t
{
    return bn::type_id<SavePoint>();
}

void SavePoint::onInteract()
{
    Interaction::onInteract();

    BN_LOG("TODO: Interact with `SavePoint`!");
}

} // namespace ut::game::cpnt::inter
