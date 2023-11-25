#include "game/cpnt/inter/TorielGoOutRuins2.hpp"

#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/EntityManager.hpp"
#include "game/task/TaskAwaiters.hpp"

#include "gen/EntityId.hpp"

namespace ut::game::cpnt::inter
{

TorielGoOutRuins2::TorielGoOutRuins2(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<TorielGoOutRuins2>(), isEnabled, triggers)
{
}

void TorielGoOutRuins2::awake(GameContext& ctx)
{
    if (ctx.state.getPlot() >= game::GamePlot::STEPPING_TILE_PUZZLE_COMPLETE)
        _entity.setDestroyed(true);
}

auto TorielGoOutRuins2::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    // prevent interacting multiple times
    setEnabled(false);

    auto* toriel = ctx.entMngr.findById(ent::gen::EntityId::toriel);
    BN_ASSERT(toriel);
    auto* torielInput = toriel->getComponent<cpnt::NpcInput>();
    BN_ASSERT(torielInput);

    cmd::MoveCmd moveCmd;
    moveCmd.directions = core::Directions::UP;
    moveCmd.movePos = {0, -2.4};
    moveCmd.checkCollision = false;
    torielInput->startOneWay(moveCmd, 17);
    co_await task::SignalAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

    toriel->setDestroyed(true);
    _entity.setDestroyed(true);
    co_return;
}

} // namespace ut::game::cpnt::inter
