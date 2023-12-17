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

RuinsFloorSwitch::RuinsFloorSwitch(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<RuinsFloorSwitch>(), isEnabled, triggers)
{
}

auto RuinsFloorSwitch::onInteract(GameContext& ctx) -> task::Task
{
    if (_isPressed)
        co_return;

    _isPressed = true;

    Interaction::onInteract(ctx);

    asset::getSfx(asset::SfxKind::SWITCH_TRIGGER)->play();

    auto* spr = _entity.getComponent<cpnt::Sprite>();
    BN_ASSERT(spr != nullptr);
    spr->setGfxIdx(FloorSwitchGfxIdx::PRESSED);

    co_return;
}

} // namespace ut::game::cpnt::inter
