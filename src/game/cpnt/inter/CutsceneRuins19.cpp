#include "game/cpnt/inter/CutsceneRuins19.hpp"

#include <bn_sound_item.h>

#include "asset/Bgm.hpp"
#include "asset/Path.hpp"
#include "asset/SfxKind.hpp"
#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/cpnt/inter/InteractionType.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/EntityManager.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

static constexpr int TORIEL_CALL_GFXIDX = 10;

CutsceneRuins19::CutsceneRuins19(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, InteractionType::CutsceneRuins19, isEnabled, triggers)
{
}

void CutsceneRuins19::awake(GameContext& ctx)
{
    _toriel = ctx.entMngr.findById(ent::gen::EntityId::toriel);
    BN_ASSERT(_toriel);
    _torielSpr = _toriel->getComponent<Sprite>();
    BN_ASSERT(_torielSpr);
    _torielSprAnim = _toriel->getComponent<SpriteAnim>();
    BN_ASSERT(_torielSprAnim);
    _torielInput = _toriel->getComponent<NpcInput>();
    BN_ASSERT(_torielInput);
    _torielWalk = _toriel->getComponent<WalkAnimCtrl>();
    BN_ASSERT(_torielWalk);

    if (ctx.state.getPlot() >= GamePlot::MET_TORIEL_AT_RUINS_END)
    {
        _toriel->setDestroyed(true);
        _entity.setDestroyed(true);
    }
}

auto CutsceneRuins19::onInteract(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    Interaction::onInteract(ctx);

    ctx.interactStack.push(InteractState::CUTSCENE);
    setEnabled(false);
    Bgm::fadeOutAndStash(50);

    task::SignalAwaiter dialogEndAwaiter({task::TaskSignal::Kind::DIALOG_END});
    task::SignalAwaiter torielWalkAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});
    task::SignalAwaiter exclamationBalloonDestroyedAwaiter(
        {task::TaskSignal::Kind::ENT_DESTROYED, (int)ent::gen::EntityId::exc_balloon});

    // Toriel talks
    ctx.msg.getSettings() =
        core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
    ctx.msg.getSettings().emotion = 1;
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::obj_torieltrigger5_260);
    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    // Toriel walks next to the tree
    _torielInput->startPath(IPath::get(gen::PathId::path_torielwalk19), 2.4);
    co_await torielWalkAwaiter;

    // disable Toriel's `walk`, `input`, `sprAnim` to prevent it from changing gfxIdx
    _torielWalk->setEnabled(false);
    _torielInput->setEnabled(false);
    _torielSprAnim->setEnabled(false);

    // Toriel calls Frisk
    getSfx(SfxKind::PHONE)->play();
    _torielSpr->setGfxIdx(TORIEL_CALL_GFXIDX);
    co_await task::TimeAwaiter(30);

    // Toriel exclamation speech balloon
    ctx.entMngr.createExclamationBalloon(_toriel->getPosition() + bn::fixed_point(3, -42), false);
    co_await exclamationBalloonDestroyedAwaiter;

    // re-enable Toriel's `walk`, `input`, `sprAnim` to animate walking
    _torielWalk->setEnabled(true);
    _torielInput->setEnabled(true);
    _torielSprAnim->setEnabled(true);

    // Toriel walks to Frisk
    _torielInput->startPath(IPath::get(gen::PathId::path_torielwalk19_3), 4);
    co_await torielWalkAwaiter;

    Bgm::play(BgmKind::FALLEN_DOWN);

    // Toriel talks
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::obj_torieltrigger5_185);
    if (ctx.state.getCurHp() == ctx.state.getMaxHp())
        ctx.msg.add(gen::TextData::obj_torieltrigger5_186);
    else if (ctx.state.getCurHp() < 4)
        ctx.msg.add(gen::TextData::obj_torieltrigger5_189);
    else
        ctx.msg.add(gen::TextData::obj_torieltrigger5_187);
    ctx.msg.add(gen::TextData::obj_torieltrigger5_190);
    ctx.msg.add(gen::TextData::obj_torieltrigger5_191);
    ctx.msg.add(gen::TextData::obj_torieltrigger5_192);
    ctx.msg.add(gen::TextData::obj_torieltrigger5_193);
    ctx.msg.add(gen::TextData::obj_torieltrigger5_194);

    ctx.state.changeHp(ctx.state.getMaxHp() - ctx.state.getCurHp());
    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    ctx.msg.getSettings().reset();

    BN_ASSERT(ctx.interactStack.top() == InteractState::CUTSCENE);
    ctx.interactStack.pop();

    ctx.state.setPlot(GamePlot::MET_TORIEL_AT_RUINS_END);
    Bgm::stop();
    Bgm::restoreAndFadeIn(16);

    // Toriel goes out of room
    _torielInput->startPath(IPath::get(gen::PathId::path_torielwalk19_4), 3.2);
    co_await torielWalkAwaiter;

    _toriel->setDestroyed(true);
    _entity.setDestroyed(true);
    co_return;
}

} // namespace ut::game::cpnt::inter
