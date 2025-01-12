#include "game/cpnt/inter/FloweyTrigger1.hpp"

#include "asset/Bgm.hpp"
#include "asset/Path.hpp"
#include "asset/SpriteAnimKind.hpp"
#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/cpnt/inter/InteractionType.hpp"
#include "game/sys/EntityManager.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

FloweyTrigger1::FloweyTrigger1(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, InteractionType::FloweyTrigger1, isEnabled, triggers)
{
}

void FloweyTrigger1::awake(GameContext& ctx)
{
    _flowey = ctx.entMngr.findById(ent::gen::EntityId::flowey);
    BN_ASSERT(_flowey);
    _toriel = ctx.entMngr.findById(ent::gen::EntityId::toriel);
    BN_ASSERT(_toriel);

    const auto& floweyPersist = ctx.state.getPersistData().Flowey;

    const bool a = floweyPersist.Alter;
    const bool b = floweyPersist.K;
    const bool c = floweyPersist.SPECIALK;

    if (a || b || c)
    {
        _noFloweyEncounter = true;
        _flowey->setDestroyed(true);
        _flowey = nullptr;
    }

    if (ctx.state.getPlot() >= GamePlot::FLOWEY_INTRO_COMPLETE)
    {
        _flowey->setDestroyed(true);
        _toriel->setDestroyed(true);
        _entity.setDestroyed(true);
    }
}

auto FloweyTrigger1::onInteract(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    Interaction::onInteract(ctx);

    // prevent multiple interaction
    setEnabled(false);

    task::SignalAwaiter dialogEndAwaiter({task::TaskSignal::Kind::DIALOG_END});
    task::SignalAwaiter torielWalkAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

    ctx.interactStack.push(InteractState::CUTSCENE);

    ctx.msg.clearMsg();

    bool noFloweyFight = _noFloweyEncounter;

    if (!_noFloweyEncounter)
    {
        auto& floweyPersist = ctx.state.getPersistData().Flowey;

        const auto g = floweyPersist.Met1;
        const bool truename = floweyPersist.truename;
        const bool IK = floweyPersist.IK;
        const bool NK = floweyPersist.NK;

        Bgm::play(BgmKind::YOUR_BEST_FRIEND);

        ctx.msg.clearMsg();
        ctx.msg.getSettings() =
            core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_FLOWEY_NORMAL);

        if (NK || IK || truename)
        {
            floweyPersist.Alter = true;
            noFloweyFight = true;

            if (NK)
            {
                ctx.msg.add(gen::TextData::obj_floweytrigger_357);
                ctx.msg.add(gen::TextData::obj_floweytrigger_358);
                ctx.msg.add(gen::TextData::obj_floweytrigger_359);
                ctx.msg.add(gen::TextData::obj_floweytrigger_360);
                ctx.msg.add(gen::TextData::obj_floweytrigger_361);
                ctx.msg.add(gen::TextData::obj_floweytrigger_362);
                ctx.msg.add(gen::TextData::obj_floweytrigger_363);
            }
            else if (IK)
            {
                ctx.msg.add(gen::TextData::obj_floweytrigger_348);
                ctx.msg.add(gen::TextData::obj_floweytrigger_349);
                ctx.msg.add(gen::TextData::obj_floweytrigger_350);
                ctx.msg.add(gen::TextData::obj_floweytrigger_351);
                ctx.msg.add(gen::TextData::obj_floweytrigger_352);
                ctx.msg.add(gen::TextData::obj_floweytrigger_353);
            }
            else // truename
            {
                ctx.msg.add(gen::TextData::obj_floweytrigger_342);
                ctx.msg.add(gen::TextData::obj_floweytrigger_343);
                ctx.msg.add(gen::TextData::obj_floweytrigger_344);
            }
        }
        else if (g == PersistData::Flowey::Met1::MET_MORE)
        {
            ctx.msg.add(gen::TextData::obj_floweytrigger_327);
        }
        else if (g == PersistData::Flowey::Met1::MET_ONCE)
        {
            ctx.msg.add(gen::TextData::obj_floweytrigger_316);
            ctx.msg.add(gen::TextData::obj_floweytrigger_317);
            ctx.msg.add(gen::TextData::obj_floweytrigger_318);
            ctx.msg.add(gen::TextData::obj_floweytrigger_319);
            ctx.msg.add(gen::TextData::obj_floweytrigger_320);
            ctx.msg.add(gen::TextData::obj_floweytrigger_321);
            ctx.msg.add(gen::TextData::obj_floweytrigger_322);
        }
        else
        {
            ctx.msg.add(gen::TextData::SCR_TEXT_291);
            ctx.msg.add(gen::TextData::SCR_TEXT_292);
            ctx.msg.add(gen::TextData::SCR_TEXT_293);
            ctx.msg.add(gen::TextData::SCR_TEXT_294);
            ctx.msg.add(gen::TextData::SCR_TEXT_295);
            ctx.msg.add(gen::TextData::SCR_TEXT_296);
            ctx.msg.add(gen::TextData::SCR_TEXT_297);
        }

        if (g == PersistData::Flowey::Met1::NEVER_MET)
            floweyPersist.Met1 = PersistData::Flowey::Met1::MET_ONCE;
        else if (g == PersistData::Flowey::Met1::MET_ONCE)
            floweyPersist.Met1 = PersistData::Flowey::Met1::MET_MORE;

        ctx.state.savePersist();

        ctx.game.startDialog();
        co_await dialogEndAwaiter;

        if (!noFloweyFight)
        {
            // TODO: Start & Wait for the fight to finish

            _flowey->setDestroyed(true);
            _flowey = nullptr;
        }
        else
        {
            co_await task::TimeAwaiter(5);

            auto* floweyAnim = _flowey->getComponent<cpnt::SpriteAnim>();
            BN_ASSERT(floweyAnim);
            task::SignalAwaiter floweyAnimEndAwaiter(
                {task::TaskSignal::Kind::SPR_ANIM_END, (int)ent::gen::EntityId::flowey});

            // Flowey sinks
            Bgm::fadeOutAndStash(10);
            floweyAnim->setCurAnimKind(SpriteAnimKind::FLOWEY_SINK_SLOW);
            co_await floweyAnimEndAwaiter;

            _flowey->setDestroyed(true);
            _flowey = nullptr;
            co_await task::TimeAwaiter(50);
        }
    }

    ctx.msg.getSettings().reset();
    ctx.msg.getSettings() =
        core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
    ctx.msg.clearMsg();

    auto* torielInput = _toriel->getComponent<NpcInput>();
    static constexpr bn::fixed MOVE_SPEED = 1.6;

    if (Bgm::getPlayingBgmKind() != BgmKind::FALLEN_DOWN)
        Bgm::play(BgmKind::FALLEN_DOWN);

    if (noFloweyFight)
    {
        // Toriel walks to Frisk
        torielInput->startPath(IPath::get(gen::PathId::path_torielwalk0), MOVE_SPEED);
        co_await torielWalkAwaiter;
        co_await task::TimeAwaiter(30);

        // Toriel talks
        ctx.msg.add(gen::TextData::obj_floweytrigger_122);
        ctx.msg.add(gen::TextData::obj_floweytrigger_123);
        ctx.msg.add(gen::TextData::obj_floweytrigger_124);
        ctx.msg.add(gen::TextData::obj_floweytrigger_125);
        ctx.msg.add(gen::TextData::obj_floweytrigger_126);
        ctx.msg.add(gen::TextData::obj_floweytrigger_127);
        ctx.msg.add(gen::TextData::obj_floweytrigger_128);
        ctx.msg.add(gen::TextData::obj_floweytrigger_129);
    }
    else
    {
        // Warp Toriel to Frisk
        _toriel->setPosition(IPath::get(gen::PathId::path_torielwalk0).getPoints().back());

        // Toriel talks
        ctx.msg.add(gen::TextData::SCR_TEXT_301);
    }

    co_await task::TimeAwaiter(2); // dialog after dialog hack... for now

    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    ctx.msg.getSettings().reset();

    torielInput->startPath(IPath::get(gen::PathId::path_torielwalk0_2), MOVE_SPEED);
    ctx.state.setPlot(GamePlot::FLOWEY_INTRO_COMPLETE);
    co_await task::TimeAwaiter(15);

    BN_ASSERT(ctx.interactStack.top() == InteractState::CUTSCENE);
    ctx.interactStack.pop();

    co_await torielWalkAwaiter;

    _toriel->setDestroyed(true);
    co_return;
}

} // namespace ut::game::cpnt::inter
