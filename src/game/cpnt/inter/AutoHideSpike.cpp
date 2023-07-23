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

void AutoHideSpike::onInteract()
{
    Interaction::onInteract();

    auto* spr = _entity.getComponent<cpnt::Sprite>();
    BN_ASSERT(spr != nullptr);

    spr->setGfxIdx(SpikeGfxIdx::HIDE);

    _spikeReappearCountdown = -1;
}

void AutoHideSpike::onInteractionCollisionExit()
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
