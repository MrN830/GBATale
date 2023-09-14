#include "game/cpnt/inter/AutoHideSpike.hpp"

#include "game/cpnt/Sprite.hpp"
#include "game/ent/Entity.hpp"

namespace ut::game::cpnt::inter
{

namespace
{

constexpr int SPIKE_APPEAR_FRAMES = 6;

enum SpikeGfxIdx
{
    APPEAR = 0,
    HIDE = 1,
};

} // namespace

AutoHideSpike::AutoHideSpike(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<AutoHideSpike>(), isEnabled, triggers)
{
}

auto AutoHideSpike::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    auto* spr = _entity.getComponent<cpnt::Sprite>();
    BN_ASSERT(spr != nullptr);

    spr->setGfxIdx(SpikeGfxIdx::HIDE);

    _spikeReappearCountdown = -1;

    co_return;
}

void AutoHideSpike::onInteractionCollisionExit(GameContext&)
{
    _spikeReappearCountdown = SPIKE_APPEAR_FRAMES;
}

void AutoHideSpike::render(GameContext&)
{
    if (_spikeReappearCountdown > 0)
    {
        if (--_spikeReappearCountdown <= 0)
        {
            auto* spr = _entity.getComponent<cpnt::Sprite>();
            BN_ASSERT(spr != nullptr);

            spr->setGfxIdx(SpikeGfxIdx::APPEAR);
        }
    }
}

} // namespace ut::game::cpnt::inter
