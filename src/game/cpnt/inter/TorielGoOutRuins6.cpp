#include "game/cpnt/inter/TorielGoOutRuins6.hpp"

#include <bn_sound_item.h>

#include "asset/Bgm.hpp"
#include "asset/Path.hpp"
#include "asset/SfxKind.hpp"
#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/cpnt/inter/InteractionType.hpp"
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
    : Interaction(entity, InteractionType::TorielGoOutRuins6, isEnabled, triggers)
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

    task::SignalAwaiter dialogEndAwaiter({task::TaskSignal::Kind::DIALOG_END});

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
    ctx.msg.getSettings() =
        core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
    ctx.msg.add(gen::TextData::SCR_TEXT_446);
    ctx.msg.add(gen::TextData::SCR_TEXT_447);
    ctx.msg.add(gen::TextData::SCR_TEXT_448);
    ctx.msg.add(gen::TextData::SCR_TEXT_449);
    ctx.msg.add(gen::TextData::SCR_TEXT_450);
    ctx.msg.add(gen::TextData::SCR_TEXT_451);
    ctx.msg.add(gen::TextData::SCR_TEXT_452);
    ctx.msg.add(gen::TextData::SCR_TEXT_453);
    ctx.msg.add(gen::TextData::SCR_TEXT_454);
    ctx.msg.add(gen::TextData::SCR_TEXT_455);
    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    // TODO: Add cell phone in the menu
    triggerColl->setEnabled(false);
    ctx.entMngr.createStalkerFlowey({128, 80});

    ctx.msg.getSettings().reset();
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

    // TODO: Temp save

    // Toriel dog calls...
    constexpr int DOG_CALL_INTERVAL = 150 * 40;
    task::TimeAwaiter dogCallTimeAwaiter(DOG_CALL_INTERVAL);
    task::SignalAwaiter interactStateFreeAwaiter({task::TaskSignal::Kind::INTERACT_STATE_FREE});

    co_await dogCallTimeAwaiter;
    if (ctx.interactStack.top() != InteractState::FREE)
    {
        co_await interactStateFreeAwaiter;
        co_await task::TimeAwaiter(1);
    }

    getSfx(SfxKind::PHONE)->play();
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::obj_tordogcall_64);
    ctx.msg.add(gen::TextData::obj_tordogcall_65);
    ctx.msg.add(gen::TextData::obj_tordogcall_66);
    ctx.msg.add(gen::TextData::obj_tordogcall_67);
    ctx.msg.add(gen::TextData::obj_tordogcall_68);
    ctx.msg.add(gen::TextData::obj_tordogcall_69);
    ctx.msg.add(gen::TextData::obj_tordogcall_70);
    ctx.msg.add(gen::TextData::obj_tordogcall_71);
    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    co_await dogCallTimeAwaiter;
    if (ctx.interactStack.top() != InteractState::FREE)
    {
        co_await interactStateFreeAwaiter;
        co_await task::TimeAwaiter(1);
    }

    getSfx(SfxKind::PHONE)->play();
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::obj_tordogcall_84);
    ctx.msg.add(gen::TextData::obj_tordogcall_85);
    ctx.msg.add(gen::TextData::obj_tordogcall_86);
    ctx.msg.add(gen::TextData::obj_tordogcall_87);
    ctx.msg.add(gen::TextData::obj_tordogcall_88);
    ctx.msg.add(gen::TextData::obj_tordogcall_89);
    ctx.msg.add(gen::TextData::obj_tordogcall_90);
    ctx.msg.add(gen::TextData::obj_tordogcall_91);
    ctx.msg.add(gen::TextData::obj_tordogcall_92);
    ctx.msg.add(gen::TextData::obj_tordogcall_93);
    ctx.msg.add(gen::TextData::obj_tordogcall_94);
    ctx.msg.add(gen::TextData::obj_tordogcall_95);
    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    co_await dogCallTimeAwaiter;
    if (ctx.interactStack.top() != InteractState::FREE)
    {
        co_await interactStateFreeAwaiter;
        co_await task::TimeAwaiter(1);
    }

    ctx.state.getFlags().dog_call_status = GameFlags::DogCallStatus::DOG_KIDNAPPED_PHONE;

    getSfx(SfxKind::PHONE)->play();
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::obj_tordogcall_109);
    ctx.msg.add(gen::TextData::obj_tordogcall_110);
    ctx.msg.add(gen::TextData::obj_tordogcall_111);
    ctx.msg.add(gen::TextData::obj_tordogcall_112);
    ctx.msg.add(gen::TextData::obj_tordogcall_113);
    ctx.msg.add(gen::TextData::obj_tordogcall_114);
    ctx.msg.add(gen::TextData::obj_tordogcall_115);
    ctx.msg.add(gen::TextData::obj_tordogcall_116);
    ctx.msg.add(gen::TextData::obj_tordogcall_117);
    ctx.msg.add(gen::TextData::obj_tordogcall_118);
    ctx.msg.add(gen::TextData::obj_tordogcall_119);
    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_INDEX, 4});
    getSfx(SfxKind::POM_BARK)->play();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_INDEX, 5});
    getSfx(SfxKind::POM_BARK)->play();
    co_await dialogEndAwaiter;

    co_await dogCallTimeAwaiter;
    if (ctx.interactStack.top() != InteractState::FREE)
    {
        co_await interactStateFreeAwaiter;
        co_await task::TimeAwaiter(1);
    }

    getSfx(SfxKind::PHONE)->play();
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::obj_tordogcall_150);
    ctx.msg.add(gen::TextData::obj_tordogcall_151);
    ctx.msg.add(gen::TextData::obj_tordogcall_152);
    ctx.msg.add(gen::TextData::obj_tordogcall_153);
    ctx.msg.add(gen::TextData::obj_tordogcall_154);
    ctx.msg.add(gen::TextData::obj_tordogcall_155);
    ctx.msg.add(gen::TextData::obj_tordogcall_156);
    ctx.msg.add(gen::TextData::obj_tordogcall_157);
    ctx.msg.add(gen::TextData::obj_tordogcall_158);
    ctx.msg.add(gen::TextData::obj_tordogcall_159);
    ctx.msg.add(gen::TextData::obj_tordogcall_160);
    ctx.msg.add(gen::TextData::obj_tordogcall_161);
    ctx.msg.add(gen::TextData::obj_tordogcall_162);
    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    co_await dogCallTimeAwaiter;
    if (ctx.interactStack.top() != InteractState::FREE)
    {
        co_await interactStateFreeAwaiter;
        co_await task::TimeAwaiter(1);
    }

    getSfx(SfxKind::PHONE)->play();
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::obj_tordogcall_176);
    ctx.msg.add(gen::TextData::obj_tordogcall_177);
    ctx.msg.add(gen::TextData::obj_tordogcall_178);
    ctx.msg.add(gen::TextData::obj_tordogcall_179);
    ctx.msg.add(gen::TextData::obj_tordogcall_180);
    ctx.msg.add(gen::TextData::obj_tordogcall_181);
    ctx.msg.add(gen::TextData::obj_tordogcall_182);
    ctx.msg.add(gen::TextData::obj_tordogcall_183);
    ctx.msg.add(gen::TextData::obj_tordogcall_184);
    ctx.msg.add(gen::TextData::obj_tordogcall_185);
    ctx.msg.add(gen::TextData::obj_tordogcall_186);
    ctx.msg.add(gen::TextData::obj_tordogcall_187);
    ctx.msg.add(gen::TextData::obj_tordogcall_188);
    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    co_await dogCallTimeAwaiter;
    if (ctx.interactStack.top() != InteractState::FREE)
    {
        co_await interactStateFreeAwaiter;
        co_await task::TimeAwaiter(1);
    }

    getSfx(SfxKind::PHONE)->play();
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::obj_tordogcall_203);
    ctx.msg.add(gen::TextData::obj_tordogcall_204);
    ctx.msg.add(gen::TextData::obj_tordogcall_205);
    ctx.msg.add(gen::TextData::obj_tordogcall_206);
    ctx.msg.add(gen::TextData::obj_tordogcall_207);
    ctx.msg.add(gen::TextData::obj_tordogcall_208);
    ctx.msg.add(gen::TextData::obj_tordogcall_209);
    ctx.msg.add(gen::TextData::obj_tordogcall_210);
    ctx.msg.add(gen::TextData::obj_tordogcall_211);
    ctx.msg.add(gen::TextData::obj_tordogcall_212);
    ctx.msg.add(gen::TextData::obj_tordogcall_213);
    ctx.msg.add(gen::TextData::obj_tordogcall_214);
    ctx.msg.add(gen::TextData::obj_tordogcall_215);
    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    co_await dogCallTimeAwaiter;
    if (ctx.interactStack.top() != InteractState::FREE)
    {
        co_await interactStateFreeAwaiter;
        co_await task::TimeAwaiter(1);
    }

    getSfx(SfxKind::PHONE)->play();
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::obj_tordogcall_230);
    ctx.msg.add(gen::TextData::obj_tordogcall_231);
    ctx.msg.add(gen::TextData::obj_tordogcall_232);
    ctx.msg.add(gen::TextData::obj_tordogcall_233);
    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    _entity.setDestroyed(true);
    co_return;
}

} // namespace ut::game::cpnt::inter
