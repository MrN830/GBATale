#include "game/cpnt/inter/RuinsSpikeDisabler.hpp"

#include "game/GameContext.hpp"
#include "game/cpnt/ev/EventComponentType.hpp"
#include "game/cpnt/ev/PlotSpike.hpp"
#include "game/sys/EntityManager.hpp"

namespace ut::game::cpnt::inter
{

RuinsSpikeDisabler::~RuinsSpikeDisabler() = default;

RuinsSpikeDisabler::RuinsSpikeDisabler(ent::Entity& entity, InteractionType interactionType, bool isEnabled,
                                       InteractionTriggers triggers)
    : Interaction(entity, interactionType, isEnabled, triggers)
{
}

void RuinsSpikeDisabler::hideAllSpikesInRoom(GameContext& ctx)
{
    auto it = ctx.entMngr.beforeBeginIter();
    do
    {
        it = ctx.entMngr.findIf(
            [](const game::ent::Entity& entity) -> bool {
                const auto* evCpnt = entity.getComponent<game::cpnt::ev::EventComponent>();
                return (evCpnt != nullptr &&
                        evCpnt->getEventComponentType() == game::cpnt::ev::EventComponentType::PlotSpike);
            },
            it);

        if (it != ctx.entMngr.endIter())
        {
            auto* evCpnt = it->getComponent<game::cpnt::ev::EventComponent>();
            BN_ASSERT(evCpnt != nullptr);
            BN_ASSERT(evCpnt->getEventComponentType() == game::cpnt::ev::EventComponentType::PlotSpike);
            auto* plotSpike = static_cast<game::cpnt::ev::PlotSpike*>(evCpnt);

            plotSpike->hideSpike();
        }

    } while (it != ctx.entMngr.endIter());
}

} // namespace ut::game::cpnt::inter
