#include "game/cpnt/inter/FloweyTrigger2.hpp"

#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "asset/SpriteAnimKind.hpp"
#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/cpnt/inter/InteractionType.hpp"
#include "game/sys/EntityManager.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

FloweyTrigger2::FloweyTrigger2(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, InteractionType::FloweyTrigger2, isEnabled, triggers)
{
}

void FloweyTrigger2::awake(GameContext& ctx)
{
    _flowey = ctx.entMngr.findById(ent::gen::EntityId::flowey);
    BN_ASSERT(_flowey);
    _floweyAnim = _flowey->getComponent<SpriteAnim>();
    BN_ASSERT(_floweyAnim);

    const auto& floweyPersist = ctx.state.getPersistData().Flowey;
    const bool a = floweyPersist.Alter;
    const bool b = floweyPersist.K;
    const bool c = floweyPersist.SPECIALK;
    const bool d = floweyPersist.alter2;

    bool q = false;
    if (a || b || c)
        q = true;
    if (ctx.state.getMurderLv() >= 2)
        q = false;
    if (d)
        q = true;

    if (q || ctx.state.getPlot() >= GamePlot::FLOWEY_JUDGMENT_COMPLETE)
    {
        _flowey->setDestroyed(true);
        _entity.setDestroyed(true);
    }
}

auto FloweyTrigger2::onInteract(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    Interaction::onInteract(ctx);

    // prevent multiple interaction
    setEnabled(false);

    task::SignalAwaiter dialogEndAwaiter({task::TaskSignal::Kind::DIALOG_END});
    task::SignalAwaiter floweyAnimEndAwaiter({task::TaskSignal::Kind::SPR_ANIM_END, (int)ent::gen::EntityId::flowey});

    auto& flags = ctx.state.getFlags();
    auto& persist = ctx.state.getPersistData();

    const auto TK = persist.Toriel.TK;
    const auto TS = persist.Toriel.TS;
    const auto FS = persist.Flowey.FloweyExplain1;

    enum class FloweyAct
    {
        ERROR = 0,

        LAUGH = 2,
        GROW_AND_LAUGH = 3,
        WONDERFUL_IDEA = 4,
    };

    FloweyAct floweyAct = FloweyAct::ERROR;

    ctx.interactStack.push(InteractState::CUTSCENE);

    ctx.msg.clearMsg();
    ctx.msg.getSettings() =
        core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_FLOWEY_NORMAL);

    if (flags.status_toriel == GameFlags::StatusToriel::KILLED)
    {
        if (ctx.state.getMurderLv() >= 2)
        {
            persist.Flowey.truename = true;
            persist.Flowey.alter2 = true;
            ctx.state.savePersist();

            ctx.msg.add(gen::TextData::obj_floweytrigger2_226);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_227);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_228);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_229);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_230);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_231);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_232);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_233);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_234);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_235);

            floweyAct = FloweyAct::WONDERFUL_IDEA;
        }
        else if (TS > 0 && TK > 0)
        {
            ctx.msg.add(gen::TextData::obj_floweytrigger2_195);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_196);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_197);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_198);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_199);

            if (FS)
            {
                ctx.msg.add(gen::TextData::obj_floweytrigger2_200);

                floweyAct = FloweyAct::LAUGH;
            }
            else
            {
                persist.Flowey.FloweyExplain1 = true;
                ctx.state.savePersist();

                ctx.msg.add(gen::TextData::obj_floweytrigger2_203);
                ctx.msg.add(gen::TextData::obj_floweytrigger2_204);
                ctx.msg.add(gen::TextData::obj_floweytrigger2_205);
                ctx.msg.add(gen::TextData::obj_floweytrigger2_206);
                ctx.msg.add(gen::TextData::obj_floweytrigger2_207);
                ctx.msg.add(gen::TextData::obj_floweytrigger2_208);
                ctx.msg.add(gen::TextData::obj_floweytrigger2_209);
                ctx.msg.add(gen::TextData::obj_floweytrigger2_210);
                ctx.msg.add(gen::TextData::obj_floweytrigger2_211);
                ctx.msg.add(gen::TextData::obj_floweytrigger2_212);
                ctx.msg.add(gen::TextData::obj_floweytrigger2_213);
                ctx.msg.add(gen::TextData::obj_floweytrigger2_214);
                ctx.msg.add(gen::TextData::obj_floweytrigger2_215);

                floweyAct = FloweyAct::GROW_AND_LAUGH;
            }
        }
        else if (TK > 2)
        {
            ctx.msg.add(gen::TextData::obj_floweytrigger2_186);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_187);

            floweyAct = FloweyAct::LAUGH;
        }
        else if (TK == 2)
        {
            ctx.msg.add(gen::TextData::obj_floweytrigger2_175);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_176);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_177);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_178);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_179);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_180);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_181);

            floweyAct = FloweyAct::LAUGH;
        }
        else
        {
            ctx.msg.add(gen::TextData::obj_floweytrigger2_162);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_163);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_164);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_165);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_166);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_167);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_168);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_169);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_170);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_171);

            floweyAct = FloweyAct::LAUGH;
        }
    }
    else if (flags.status_toriel == GameFlags::StatusToriel::SPARED)
    {
        ctx.msg.add(gen::TextData::obj_floweytrigger2_245);
        ctx.msg.add(gen::TextData::obj_floweytrigger2_246);
        ctx.msg.add(gen::TextData::obj_floweytrigger2_247);
        ctx.msg.add(gen::TextData::obj_floweytrigger2_248);
        ctx.msg.add(gen::TextData::obj_floweytrigger2_249);
        ctx.msg.add(gen::TextData::obj_floweytrigger2_250);

        if (TK > 0 && !FS)
        {
            persist.Flowey.FloweyExplain1 = true;
            ctx.state.savePersist();

            ctx.msg.add(gen::TextData::obj_floweytrigger2_283);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_284);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_285);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_286);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_287);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_288);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_289);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_290);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_291);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_292);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_293);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_294);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_295);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_296);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_297);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_298);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_299);

            floweyAct = FloweyAct::GROW_AND_LAUGH;
        }
        else if (ctx.state.getKills() == 0)
        {
            ctx.msg.add(gen::TextData::obj_floweytrigger2_264);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_265);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_266);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_267);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_268);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_269);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_270);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_271);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_272);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_273);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_274);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_275);

            floweyAct = FloweyAct::GROW_AND_LAUGH;
        }
        else
        {
            ctx.msg.add(gen::TextData::obj_floweytrigger2_253);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_254);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_255);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_256);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_257);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_258);
            ctx.msg.add(gen::TextData::obj_floweytrigger2_259);

            floweyAct = FloweyAct::LAUGH;
        }
    }
    else
    {
        ctx.msg.add(gen::TextData::obj_floweytrigger2_158);

        floweyAct = FloweyAct::ERROR;
    }

    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    ctx.msg.getSettings().reset();

    if (floweyAct == FloweyAct::LAUGH || floweyAct == FloweyAct::WONDERFUL_IDEA)
    {
        const bool wonderful = (floweyAct == FloweyAct::WONDERFUL_IDEA);

        getSfx(wonderful ? SfxKind::WONDERFUL_IDEA : SfxKind::FLOWEY_LAUGH)->play();
        _floweyAnim->setCurAnimKind(SpriteAnimKind::FLOWEY_SMALL_LAUGH);
        co_await task::TimeAwaiter(wonderful ? 80 : 150);
    }
    else if (floweyAct == FloweyAct::GROW_AND_LAUGH)
    {
        _floweyAnim->setCurAnimKind(SpriteAnimKind::FLOWEY_GROW);
        co_await floweyAnimEndAwaiter;
        co_await task::TimeAwaiter(30);

        getSfx(SfxKind::FLOWEY_LAUGH)->play();
        _floweyAnim->setCurAnimKind(SpriteAnimKind::FLOWEY_BIG_LAUGH);
        co_await task::TimeAwaiter(150);

        _floweyAnim->setCurAnimKind(SpriteAnimKind::FLOWEY_SHRINK);
        co_await floweyAnimEndAwaiter;
    }
    else
    {
        BN_ERROR("status_toriel=", (int)flags.status_toriel, ", TK=", TK, ", TS=", TS, ", FS=", FS);
    }

    ctx.state.setPlot(GamePlot::FLOWEY_JUDGMENT_COMPLETE);
    BN_ASSERT(ctx.interactStack.top() == InteractState::CUTSCENE);
    ctx.interactStack.pop();

    _floweyAnim->setCurAnimKind(SpriteAnimKind::FLOWEY_SINK);
    co_await floweyAnimEndAwaiter;

    _flowey->setDestroyed(true);
    _entity.setDestroyed(true);
    co_return;
}

} // namespace ut::game::cpnt::inter
