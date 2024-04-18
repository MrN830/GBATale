#include "game/cpnt/inter/TalkTorielRuins3.hpp"

#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/inter/InteractionType.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

TalkTorielRuins3::TalkTorielRuins3(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, InteractionType::TalkTorielRuins3, isEnabled, triggers)
{
}

auto TalkTorielRuins3::onInteract(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    Interaction::onInteract(ctx);

    const auto plot = ctx.state.getPlot();

    ctx.msg.getSettings() =
        core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
    ctx.msg.getSettings().emotion = 2;

    ctx.msg.clearMsg();
    if (plot == GamePlot::TORIEL_WAITING_AT_SWITCH_PUZZLE)
    {
        if (_hasTalked)
        {
            ctx.msg.add(gen::TextData::SCR_TEXT_334);
            ctx.msg.add(gen::TextData::SCR_TEXT_335);
        }
        else
        {
            ctx.msg.add(gen::TextData::SCR_TEXT_330);
        }
    }
    else if (plot == GamePlot::FIRST_SWITCH_FLIPPED)
    {
        const bool hardmode = ctx.state.getFlags().hardmode;

        if (_hasTalked)
        {
            if (hardmode)
                ctx.msg.add(gen::TextData::SCR_TEXT_352);
            else
            {
                ctx.msg.add(gen::TextData::SCR_TEXT_347);
                ctx.msg.add(gen::TextData::SCR_TEXT_348);
            }
        }
        else
        {
            if (hardmode)
                ctx.msg.add(gen::TextData::SCR_TEXT_342);
            else
                ctx.msg.add(gen::TextData::SCR_TEXT_339);
        }
    }
    else
        BN_ERROR("Invalid plot=", (int)plot);

    _hasTalked = true;
    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    ctx.msg.getSettings().reset();
    co_return;
}

void TalkTorielRuins3::setTalked(bool hasTalked)
{
    _hasTalked = hasTalked;
}

} // namespace ut::game::cpnt::inter
