#include "game/cpnt/inter/RuinsFloorSwitch.hpp"

#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/ent/Entity.hpp"

namespace ut::game::cpnt::inter
{

namespace
{

enum FloorSwitchGfxIdx
{
    NOT_PRESSED = 0,
    PRESSED = 1,
};

} // namespace

RuinsFloorSwitch::RuinsFloorSwitch(ent::Entity& entity, InteractionTriggers triggers) : Interaction(entity, triggers)
{
}

auto RuinsFloorSwitch::getInteractionType() const -> bn::type_id_t
{
    return bn::type_id<RuinsFloorSwitch>();
}

void RuinsFloorSwitch::onInteract()
{
    if (isPressed)
        return;

    isPressed = true;

    Interaction::onInteract();

    asset::getSfx(asset::SfxKind::SWITCH_TRIGGER)->play();

    auto* spr = _entity.getComponent<cpnt::Sprite>();
    BN_ASSERT(spr != nullptr);
    spr->setGfxIdx(FloorSwitchGfxIdx::PRESSED);
}

} // namespace ut::game::cpnt::inter
