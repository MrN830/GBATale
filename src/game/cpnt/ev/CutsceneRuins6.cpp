#include "game/cpnt/ev/CutsceneRuins6.hpp"

#include "asset/Bgm.hpp"
#include "asset/Path.hpp"
#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/cpnt/ev/EventComponentType.hpp"
#include "game/ent/Entity.hpp"
#include "gen/EntityId.hpp"
#include "scene/Game.hpp"

#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::ev
{

CutsceneRuins6::CutsceneRuins6(ent::Entity& entity, bool isEnabled, bool isAutoFire)
    : EventComponent(entity, EventComponentType::CutsceneRuins6, isEnabled, isAutoFire)
{
    BN_ASSERT(entity.getId() == ent::gen::EntityId::toriel);
}

void CutsceneRuins6::awake(GameContext& ctx)
{
    if (ctx.state.getPlot() >= GamePlot::TORIEL_LEFT)
        _entity.setDestroyed(true);
}

auto CutsceneRuins6::onEvent(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    auto* torielInput = _entity.getComponent<NpcInput>();
    BN_ASSERT(torielInput);
    auto* torielWalk = _entity.getComponent<WalkAnimCtrl>();
    BN_ASSERT(torielWalk);
    auto* plotWall = ctx.entMngr.findById(ent::gen::EntityId::plot_wall);
    BN_ASSERT(plotWall);
    auto* plotWallColl = plotWall->getComponent<ColliderPack>();
    BN_ASSERT(plotWallColl);

    Bgm::fadeOutAndStash(10);
    torielWalk->setStandStillDir(core::Directions::LEFT);
    plotWallColl->setEnabled(true);

    ctx.msg.getSettings() =
        core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);

    // Toriel talks
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::SCR_TEXT_459);
    ctx.msg.add(gen::TextData::SCR_TEXT_460);
    ctx.msg.add(gen::TextData::SCR_TEXT_461);
    ctx.msg.add(gen::TextData::SCR_TEXT_462);
    ctx.msg.add(gen::TextData::SCR_TEXT_463);
    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    ctx.msg.getSettings().reset();

    // unnecessary tension
    Bgm::play(BgmKind::UNNECESSARY_TENSION);
    ctx.state.setPlot(GamePlot::UNNECESSARY_TENSION);

    // Toriel runs to the long hallway
    cmd::MoveCmd moveCmd;
    moveCmd.directions = core::Directions::RIGHT;
    moveCmd.movePos = {3.2, 0};
    moveCmd.checkCollision = false;
    torielInput->startOneWay(moveCmd, 4 * 30);
    co_await task::SignalAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

    // warp Toriel to the behind the pillar
    const auto pillarPathPoints = IPath::get(gen::PathId::path_torielwalk6).getPoints();
    BN_ASSERT(!pillarPathPoints.empty());
    _entity.setPosition(pillarPathPoints[0]);
    torielWalk->setStandStillDir(core::Directions::DOWN);
    co_return;
}

} // namespace ut::game::cpnt::ev
