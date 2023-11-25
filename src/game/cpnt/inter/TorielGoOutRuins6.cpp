#include "game/cpnt/inter/TorielGoOutRuins6.hpp"

#include "asset/Bgm.hpp"
#include "asset/Path.hpp"
#include "core/DialogSettings.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/EntityManager.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

TorielGoOutRuins6::TorielGoOutRuins6(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<TorielGoOutRuins6>(), isEnabled, triggers)
{
}

void TorielGoOutRuins6::awake(GameContext& ctx)
{
    if (ctx.state.getPlot() >= GamePlot::TORIEL_LEFT)
        _entity.setDestroyed(true);
}

auto TorielGoOutRuins6::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    using namespace ut::asset;

    ctx.interactStack.push(InteractState::CUTSCENE);

    task::SignalAwaiter torielWalkAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

    auto* toriel = ctx.entMngr.findById(ent::gen::EntityId::toriel);
    BN_ASSERT(toriel);
    auto* torielInput = toriel->getComponent<NpcInput>();
    BN_ASSERT(torielInput);
    auto* torielWalk = toriel->getComponent<WalkAnimCtrl>();
    BN_ASSERT(torielWalk);
    auto* triggerColl = _entity.getComponent<ColliderPack>();
    BN_ASSERT(triggerColl);
    auto* plotWall = ctx.entMngr.findById(ent::gen::EntityId::plot_wall);
    BN_ASSERT(plotWall);
    auto* plotWallColl = plotWall->getComponent<ColliderPack>();
    BN_ASSERT(plotWallColl);

    ctx.state.setPlot(GamePlot::TORIEL_LEFT);
    plotWallColl->setEnabled(false);

    // Bgm F.O.
    Bgm::fadeOutAndStash(10);

    // Toriel walks out of the pillar
    static constexpr bn::fixed MOVE_SPEED = 2.4;
    const auto& torielPath = IPath::get(gen::PathId::path_torielwalk6);
    torielInput->startPath(torielPath, MOVE_SPEED);
    co_await torielWalkAwaiter;

    // Bgm start
    Bgm::play(BgmKind::FALLEN_DOWN);

    torielWalk->setStandStillDir(core::Directions::LEFT);

    // Toriel talks
    ctx.msgSettings = core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_446));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_447));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_448));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_449));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_450));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_451));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_452));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_453));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_454));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_455));
    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    // TODO: Add cell phone in the menu
    triggerColl->setEnabled(false);
    ctx.entMngr.createStalkerFlowey({128, 80});

    ctx.msgSettings.reset();
    BN_ASSERT(ctx.interactStack.top() == InteractState::CUTSCENE);
    ctx.interactStack.pop();

    // Toriel walks out of room
    cmd::MoveCmd moveCmd = {
        .directions = core::Directions::RIGHT,
        .movePos = {MOVE_SPEED, 0},
        .checkCollision = false,
    };
    torielInput->startOneWay(moveCmd, 4 * 30);
    co_await torielWalkAwaiter;

    toriel->setDestroyed(true);
    _entity.setDestroyed(true);
    co_return;
}

} // namespace ut::game::cpnt::inter
