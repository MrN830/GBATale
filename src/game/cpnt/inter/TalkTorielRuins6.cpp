#include "game/cpnt/inter/TalkTorielRuins6.hpp"

#include "game/GameContext.hpp"
#include "game/ent/Entity.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

TalkTorielRuins6::TalkTorielRuins6(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<TalkTorielRuins6>(), isEnabled, triggers)
{
    BN_ASSERT(_entity.getId() == ent::gen::EntityId::toriel);
}

auto TalkTorielRuins6::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    using namespace ut::asset;

    ctx.msgSettings = core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
    ctx.msgSettings.emotion = 1;

    ctx.msg.clear();
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_442));
    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    ctx.msgSettings.reset();
    co_return;
}

} // namespace ut::game::cpnt::inter
