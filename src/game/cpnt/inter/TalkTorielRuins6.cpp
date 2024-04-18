#include "game/cpnt/inter/TalkTorielRuins6.hpp"

#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/cpnt/inter/InteractionType.hpp"
#include "game/ent/Entity.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

TalkTorielRuins6::TalkTorielRuins6(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, InteractionType::TalkTorielRuins6, isEnabled, triggers)
{
    BN_ASSERT(_entity.getId() == ent::gen::EntityId::toriel);
}

auto TalkTorielRuins6::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    using namespace ut::asset;

    ctx.msg.getSettings() =
        core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
    ctx.msg.getSettings().emotion = 1;

    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::SCR_TEXT_442);
    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    ctx.msg.getSettings().reset();
    co_return;
}

} // namespace ut::game::cpnt::inter
