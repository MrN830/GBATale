#include "game/cpnt/ev/CutsceneRuins5.hpp"

#include "asset/Path.hpp"
#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/cpnt/ev/EventComponentType.hpp"
#include "game/cpnt/inter/Interaction.hpp"
#include "game/ent/Entity.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::ev
{

CutsceneRuins5::CutsceneRuins5(ent::Entity& entity, bool isEnabled, bool isAutoFire)
    : EventComponent(entity, EventComponentType::CutsceneRuins5, isEnabled, isAutoFire)
{
    BN_ASSERT(_entity.getId() == ent::gen::EntityId::toriel);
}

void CutsceneRuins5::awake(GameContext& ctx)
{
    const auto plot = ctx.state.getPlot();
    if ( // plot < GamePlot::DUMMY_DEFEATED ||
        plot > GamePlot::TORIEL_AT_SPIKE_PUZZLE)
        _entity.setDestroyed(true);

    _frisk = ctx.entMngr.findById(ent::gen::EntityId::frisk);
    BN_ASSERT(_frisk);
    _torielInput = _entity.getComponent<NpcInput>();
    BN_ASSERT(_torielInput);
    _torielWalk = _entity.getComponent<cpnt::WalkAnimCtrl>();
    BN_ASSERT(_torielWalk);
}

void CutsceneRuins5::update(GameContext& ctx)
{
    EventComponent::update(ctx);

    if (_torielInput->isDone())
        return;

    const auto friskX = _frisk->getPosition().x();
    const auto torielX = _entity.getPosition().x();

    if (friskX < torielX - 32)
    {
        if (!_torielTooFarTrigger)
        {
            _torielTooFarTrigger = true;
            _torielPauseCountdown = 30;
        }
    }
    else
    {
        if (friskX > torielX - 8)
            _torielInput->setPathSpeed(4);
        else if (friskX > torielX - 20)
            _torielInput->setPathSpeed(3);
        else
            _torielInput->setPathSpeed(2);

        _torielPauseCountdown = 24;
        _torielInput->resume();
    }

    if (_torielPauseCountdown > 0 && --_torielPauseCountdown == 0)
    {
        if (!_torielInput->isPaused())
        {
            _torielInput->pause();
            _torielWalk->setStandStillDir(core::Directions::LEFT);
        }
        _torielTooFarTrigger = false;
    }
}

auto CutsceneRuins5::onEvent(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    auto* torielInteract = _entity.getComponent<inter::Interaction>();
    BN_ASSERT(torielInteract);
    auto* plotWall = ctx.entMngr.findById(ent::gen::EntityId::plot_wall);
    BN_ASSERT(plotWall);
    auto* plotWallColl = plotWall->getComponent<ColliderPack>();
    BN_ASSERT(plotWallColl);

    plotWallColl->setEnabled(true);

    ctx.msg.getSettings() =
        core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);

    // Toriel talks
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::SCR_TEXT_428);
    ctx.msg.add(gen::TextData::SCR_TEXT_429);
    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    ctx.msg.getSettings().reset();

    // Toriel walks to the in front of the spikes
    _torielInput->startPath(IPath::get(gen::PathId::path_torielwalk5), 2);
    _torielInput->pause();
    _torielWalk->setStandStillDir(core::Directions::RIGHT);
    co_await task::SignalAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

    torielInteract->setEnabled(true);
    setEnabled(false);
    co_return;
}

} // namespace ut::game::cpnt::ev
