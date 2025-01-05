#include "game/cpnt/inter/TalkTorielRuins4.hpp"

#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/cpnt/inter/InteractionType.hpp"
#include "scene/Game.hpp"

#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

TalkTorielRuins4::TalkTorielRuins4(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, InteractionType::TalkTorielRuins4, isEnabled, triggers)
{
}

auto TalkTorielRuins4::onInteract(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    Interaction::onInteract(ctx);

    ctx.msg.getSettings() =
        core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
    ctx.msg.getSettings().emotion = 2;

    ctx.msg.clearMsg();
    if (_hasTalked)
    {
        ctx.msg.add(gen::TextData::SCR_TEXT_423);
        ctx.msg.add(gen::TextData::SCR_TEXT_424);
    }
    else
    {
        ctx.msg.add(gen::TextData::SCR_TEXT_411);
        ctx.msg.add(gen::TextData::SCR_TEXT_412);
        ctx.msg.add(gen::TextData::SCR_TEXT_413);
        ctx.msg.add(gen::TextData::SCR_TEXT_414);
        ctx.msg.add(gen::TextData::SCR_TEXT_415);
        ctx.msg.add(gen::TextData::SCR_TEXT_416);
        ctx.msg.add(gen::TextData::SCR_TEXT_417);
        ctx.msg.add(gen::TextData::SCR_TEXT_418);
        ctx.msg.add(gen::TextData::SCR_TEXT_419);

        _hasTalked = true;
    }

    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    ctx.msg.getSettings().reset();
    co_return;
}

} // namespace ut::game::cpnt::inter
