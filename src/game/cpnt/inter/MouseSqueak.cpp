#include "game/cpnt/inter/MouseSqueak.hpp"

#include <bn_sound.h>
#include <bn_sound_item.h>

#include "game/cpnt/inter/InteractionType.hpp"

#include "asset/SfxKind.hpp"

namespace ut::game::cpnt::inter
{

MouseSqueak::MouseSqueak(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, InteractionType::MouseSqueak, isEnabled, triggers)
{
}

auto MouseSqueak::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    bn::sound::stop_all();
    asset::getSfx(asset::SfxKind::MOUSE_SQUEAK)->play();

    co_return;
}

} // namespace ut::game::cpnt::inter
