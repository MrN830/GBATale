#include "game/cpnt/ev/TorielGoOutRuins1.hpp"

#include "asset/Path.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/EntityManager.hpp"
#include "game/task/TaskAwaiters.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"

namespace ut::game::cpnt::ev
{

static constexpr int DIST_DIV = 2;

TorielGoOutRuins1::TorielGoOutRuins1(ent::Entity& entity, bool isEnabled, bool isAutoFire)
    : EventComponent(entity, bn::type_id<TorielGoOutRuins1>(), isEnabled, isAutoFire)
{
}

void TorielGoOutRuins1::awake(GameContext& ctx)
{
    if (ctx.state.getPlot() >= game::GamePlot::FIRST_SAVE)
        _entity.setDestroyed(true);

    _frisk = ctx.entMngr.findById(ent::gen::EntityId::frisk);
    BN_ASSERT(_frisk);
    _torielInput = _entity.getComponent<cpnt::NpcInput>();
    BN_ASSERT(_torielInput);
    _torielWalk = _entity.getComponent<cpnt::WalkAnimCtrl>();
    BN_ASSERT(_torielWalk);
}

void TorielGoOutRuins1::update(GameContext& ctx)
{
    EventComponent::update(ctx);

    if (_torielInput->isDone())
        return;

    const auto friskY = _frisk->getPosition().y();
    const auto torielY = _entity.getPosition().y();

    if (friskY > torielY + 64 / DIST_DIV)
    {
        if (!_torielTooFarTrigger)
        {
            _torielTooFarTrigger = true;
            _torielPauseCountdown = 30 / DIST_DIV;
        }
    }
    else
    {
        if (friskY < torielY + 40 / DIST_DIV)
            _torielInput->setPathSpeed(3.2);
        else if (friskY < torielY + 52 / DIST_DIV)
            _torielInput->setPathSpeed(2.4);
        else
            _torielInput->setPathSpeed(1.6);

        _torielPauseCountdown = 24 / DIST_DIV;
        _torielInput->resume();
    }

    if (_torielPauseCountdown > 0 && --_torielPauseCountdown == 0)
    {
        _torielInput->pause();
        _torielWalk->setStandStillDir(core::Directions::DOWN);
        _torielTooFarTrigger = false;
    }
}

auto TorielGoOutRuins1::onEvent(GameContext& ctx) -> task::Task
{
    const auto& torielPath = asset::IPath::get(asset::gen::PathId::path_torielwalk1);
    _torielInput->startPath(torielPath, 1.6);
    co_await task::SignalAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

    ctx.state.setPlot(GamePlot::FIRST_SAVE);
    ctx.entMngr.createStalkerFlowey({102, 352});
    _entity.setDestroyed(true);
    co_return;
}

} // namespace ut::game::cpnt::ev
