#include "game/cpnt/inter/TalkDummy.hpp"

#include "asset/Path.hpp"
#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/inter/InteractionType.hpp"
#include "game/ent/Entity.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

TalkDummy::TalkDummy(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, InteractionType::TalkDummy, isEnabled, triggers)
{
}

void TalkDummy::awake(GameContext& ctx)
{
    if (ctx.state.getFlags().status_dummy == GameFlags::StatusDummy::KILLED)
        _entity.setDestroyed(true);
    else if (ctx.state.getPlot() >= GamePlot::DUMMY_DEFEATED)
        setEnabled(false);
}

auto TalkDummy::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    // prevent multiple interaction
    setEnabled(false);

    auto* toriel = ctx.entMngr.findById(ent::gen::EntityId::toriel);
    BN_ASSERT(toriel);
    auto* torielInput = toriel->getComponent<NpcInput>();
    BN_ASSERT(torielInput);
    auto* torielInteract = toriel->getComponent<Interaction>();
    BN_ASSERT(torielInteract);

    auto& flags = ctx.state.getFlags();

    flags.spared_last = false;
    flags.escaped_last = false;
    flags.killed_last = true; // TODO: set to `false` after implementing dummy battle
    flags.bored_last = false;

    // TODO: start & co_await for dummy battle to finish

    if (flags.killed_last)
    {
        flags.status_dummy = GameFlags::StatusDummy::KILLED;
        _entity.setActive(false);
    }

    ctx.interactStack.push(InteractState::CUTSCENE);
    co_await task::TimeAwaiter(15);

    using TextData = asset::gen::TextData;
    ctx.msg.getSettings() =
        core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);

    // Toriel talks
    ctx.msg.clearMsg();
    if (flags.bored_last)
    {
        ctx.msg.add(TextData::SCR_TEXT_398);
        ctx.msg.add(TextData::SCR_TEXT_399);
        ctx.msg.add(TextData::SCR_TEXT_400);
    }
    else if (flags.escaped_last)
    {
        ctx.msg.add(TextData::SCR_TEXT_388);
        ctx.msg.add(TextData::SCR_TEXT_389);
        ctx.msg.add(TextData::SCR_TEXT_390);
        ctx.msg.add(TextData::SCR_TEXT_391);
        ctx.msg.add(TextData::SCR_TEXT_392);
        ctx.msg.add(TextData::SCR_TEXT_393);
        ctx.msg.add(TextData::SCR_TEXT_394);
    }
    else if (flags.spared_last)
    {
        ctx.msg.add(TextData::SCR_TEXT_384);
    }
    else if (flags.killed_last)
    {
        ctx.msg.add(TextData::SCR_TEXT_379);
        ctx.msg.add(TextData::SCR_TEXT_380);
    }
    else
    {
        ctx.msg.add(TextData::SCR_TEXT_2089);

        ctx.game.startDialog();
        co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

        BN_ERROR("No battle flag set after dummy fight");
    }

    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    ctx.msg.getSettings().reset();

    ctx.state.setPlot(GamePlot::TORIEL_AT_SPIKE_PUZZLE);

    torielInteract->setEnabled(false);

    BN_ASSERT(ctx.interactStack.top() == InteractState::CUTSCENE);
    ctx.interactStack.pop();

    // Toriel walks out of room
    torielInput->startPath(asset::IPath::get(asset::gen::PathId::path_torielwalk4_2), 2.4);
    co_await task::SignalAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

    toriel->setDestroyed(true);
    if (flags.killed_last)
        _entity.setDestroyed(true);

    co_return;
}

} // namespace ut::game::cpnt::inter
