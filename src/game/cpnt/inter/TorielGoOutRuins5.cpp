#include "game/cpnt/inter/TorielGoOutRuins5.hpp"

#include "asset/Path.hpp"
#include "asset/SpriteAnimKind.hpp"
#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/Sprite.hpp"
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

TorielGoOutRuins5::TorielGoOutRuins5(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<TorielGoOutRuins5>(), isEnabled, triggers)
{
    BN_ASSERT(_entity.getId() == ent::gen::EntityId::toriel);
}

auto TorielGoOutRuins5::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    using namespace ut::asset;

    auto* torielWalk = _entity.getComponent<WalkAnimCtrl>();
    BN_ASSERT(torielWalk);
    auto* torielInput = _entity.getComponent<NpcInput>();
    BN_ASSERT(torielInput);
    auto* frisk = ctx.entMngr.findById(ent::gen::EntityId::frisk);
    BN_ASSERT(frisk);
    auto* friskSpr = frisk->getComponent<Sprite>();
    BN_ASSERT(friskSpr);
    auto* plotWall = ctx.entMngr.findById(ent::gen::EntityId::plot_wall);
    BN_ASSERT(plotWall);
    auto* plotWallColl = plotWall->getComponent<ColliderPack>();
    BN_ASSERT(plotWallColl);

    const auto& spikePath = IPath::get(gen::PathId::path_torielwalk5_2);

    // prevent interacting multiple times
    setEnabled(false);

    ctx.interactStack.push(InteractState::CUTSCENE);

    ctx.msg.getSettings() =
        core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);

    // 1. Toriel talks
    task::SignalAwaiter talkAwaiter({task::TaskSignal::Kind::DIALOG_END});
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::SCR_TEXT_433);
    ctx.msg.add(gen::TextData::SCR_TEXT_434);
    ctx.game.startDialog();
    co_await talkAwaiter;

    ctx.state.setPlot(GamePlot::CROSSING_SPIKE_PUZZLE);
    plotWallColl->setEnabled(false);

    // disable Frisk sprite
    friskSpr->setEnabled(false);

    // set Toriel & Frisk hand hold sprite
    torielWalk->registerWalkAnimKind(WalkAnimKind::TORIEL_HANDHOLD_FRISK);
    torielWalk->setStandStillDir(core::Directions::RIGHT);
    const auto prevCamDiff = ctx.camMngr.getCamFollowEntityDiff();
    ctx.camMngr.setCamFollowEntity(&_entity);
    ctx.camMngr.setCamFollowEntityDiff(ctx.camMngr.getCamFollowEntityDiff() + bn::fixed_point(-12, 0));

    // 2. Toriel & Frisk walk across the spikes
    task::SignalAwaiter walkAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});
    torielInput->startPath(spikePath, 0.8);
    co_await walkAwaiter;

    ctx.state.setPlot(GamePlot::SPIKE_PUZZLE_COMPLETE);

    // set Toriel walk alone sprite
    torielWalk->registerWalkAnimKind(WalkAnimKind::TORIEL);
    torielWalk->setStandStillDir(core::Directions::LEFT);
    _entity.setPosition(_entity.getPosition() + bn::fixed_point(4, 0));

    // enable Frisk sprite
    frisk->setPosition(_entity.getPosition() + bn::fixed_point(-20, 0));
    friskSpr->setEnabled(true);
    ctx.camMngr.setCamFollowEntity(frisk);
    ctx.camMngr.setCamFollowEntityDiff(prevCamDiff);

    // 3. Toriel finally talks
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::SCR_TEXT_438);
    ctx.game.startDialog();
    co_await talkAwaiter;

    ctx.msg.getSettings().reset();

    BN_ASSERT(ctx.interactStack.top() == InteractState::CUTSCENE);
    ctx.interactStack.pop();

    // 4. Toriel goes out of room
    cmd::MoveCmd moveCmd;
    moveCmd.directions = core::Directions::RIGHT;
    moveCmd.movePos = {2.4, 0};
    moveCmd.checkCollision = false;
    torielInput->startOneWay(moveCmd, 2 * 30);
    co_await walkAwaiter;

    _entity.setDestroyed(true);
    co_return;
}

} // namespace ut::game::cpnt::inter
