#include "game/cpnt/inter/CutsceneBasement1Block.hpp"

#include "asset/Path.hpp"
#include "asset/SpriteAnimKind.hpp"
#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/EntityManager.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

CutsceneBasement1Block::CutsceneBasement1Block(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<CutsceneBasement1Block>(), isEnabled, triggers)
{
}

void CutsceneBasement1Block::awake(GameContext& ctx)
{
    if (ctx.state.getPlot() > GamePlot::I_HAVE_TO_DO_SOMETHING)
        _entity.setDestroyed(true);
    else
    {
        _frisk = ctx.entMngr.findById(ent::gen::EntityId::frisk);
        BN_ASSERT(_frisk);
        _friskSpr = _frisk->getComponent<Sprite>();
        BN_ASSERT(_friskSpr);
        _friskInput = _frisk->getComponent<NpcInput>();
        BN_ASSERT(_friskInput);

        _toriel = ctx.entMngr.findById(ent::gen::EntityId::toriel);
        BN_ASSERT(_toriel);
        _torielSpr = _toriel->getComponent<Sprite>();
        BN_ASSERT(_torielSpr);
        _torielInput = _toriel->getComponent<NpcInput>();
        BN_ASSERT(_torielInput);
        _torielWalk = _toriel->getComponent<WalkAnimCtrl>();
        BN_ASSERT(_torielWalk);

        // TODO: set Toriel sprite invisible
        _torielSpr->setEnabled(false);
    }
}

auto CutsceneBasement1Block::onInteract(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    Interaction::onInteract(ctx);

    task::SignalAwaiter torielWalkAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

    const auto plot = ctx.state.getPlot();
    auto& flags = ctx.state.getFlags();

    flags.status_toriel = GameFlags::StatusToriel::LEAVE_RUINS;
    ctx.interactStack.push(InteractState::CUTSCENE);

    // Toriel runs to Frisk
    _torielSpr->setEnabled(true);
    _torielInput->startPath(IPath::get(gen::PathId::path_torielwalkbasement1), 4);
    co_await torielWalkAwaiter;

    // Toriel talks
    ctx.msg.getSettings() =
        core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
    ctx.msg.getSettings().emotion = 2;
    ctx.msg.clearMsg();
    if (plot < GamePlot::TORIEL_READING)
    {
        ctx.msg.add(gen::TextData::obj_torieltrigger8_90);
        ctx.msg.add(gen::TextData::obj_torieltrigger8_91);
    }
    else
    {
        constexpr bn::array<gen::TextData, 10> DISOBEY_TEXTDATAS = {
            gen::TextData::obj_torieltrigger8_77, gen::TextData::obj_torieltrigger8_78,
            gen::TextData::obj_torieltrigger8_79, gen::TextData::obj_torieltrigger8_80,
            gen::TextData::obj_torieltrigger8_81, gen::TextData::obj_torieltrigger8_82,
            gen::TextData::obj_torieltrigger8_83, gen::TextData::obj_torieltrigger8_84,
            gen::TextData::obj_torieltrigger8_85, gen::TextData::obj_torieltrigger8_86,
        };
        const int idx = bn::clamp((int)flags.disobeyed_toriel, 0, DISOBEY_TEXTDATAS.size() - 1);
        const auto& textEn = gen::getTextEn(DISOBEY_TEXTDATAS[idx]);

        flags.disobeyed_toriel += 1;
        ctx.msg.add(textEn);
    }
    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    ctx.msg.getSettings().reset();

    // Toriel & Frisk goes back to upper floor
    _friskSpr->setEnabled(false);
    _friskInput->setEnabled(true);
    _torielWalk->registerWalkAnimKind(WalkAnimKind::TORIEL_HANDHOLD_FRISK);
    _torielWalk->setStandStillDir(core::Directions::LEFT);
    const auto& path = IPath::get(gen::PathId::path_torielwalkbasement2);
    _friskInput->startPath(path, 4);
    _torielInput->startPath(path, 4);

    co_return;
}

} // namespace ut::game::cpnt::inter
