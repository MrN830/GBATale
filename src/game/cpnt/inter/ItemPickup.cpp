#include "game/cpnt/inter/ItemPickup.hpp"

#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/cpnt/inter/InteractionType.hpp"
#include "game/ent/Entity.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

ItemPickup::ItemPickup(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, InteractionType::ItemPickup, isEnabled, triggers)
{
}

void ItemPickup::awake(GameContext& ctx)
{
    destroyIfAlreadyGot(ctx);
}

auto ItemPickup::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    auto& state = ctx.state;

    auto& flags = state.getFlags();
    auto& items = state.getItems();
    const auto room = state.getRoom();

    ctx.msg.clearMsg();

    // TODO: Add more items
    switch (room)
    {
        using namespace ut::asset;

    case RoomKind::ROOM_RUINS14:
        if (items.full())
            ctx.msg.add(gen::TextData::SCR_TEXT_1772);
        else
        {
            items.push_back(ItemKind::FADED_RIBBON);
            flags.got_ribbon = true;

            ctx.msg.add(gen::TextData::SCR_TEXT_1771);
        }
        break;

    case RoomKind::ROOM_RUINS18OLD:
        if (items.full())
            ctx.msg.add(gen::TextData::SCR_TEXT_1818);
        else
        {
            items.push_back(ItemKind::TOY_KNIFE);
            flags.got_toyknife = true;

            ctx.msg.add(gen::TextData::SCR_TEXT_1817);
        }
        break;

    case RoomKind::ROOM_ASRIELROOM:
        if (items.full())
            ctx.msg.add(gen::TextData::SCR_TEXT_1843);
        else
        {
            items.push_back(flags.hardmode ? ItemKind::SNAIL_PIE : ItemKind::BUTTERSCOTCH_PIE);
            flags.got_bscotch_pie = GameFlags::GotBscotchPie::GOT;

            if (flags.hardmode)
                ctx.msg.add(gen::TextData::SCR_TEXT_1853);
            else
                ctx.msg.add(gen::TextData::SCR_TEXT_1842);
        }
        break;

    default:
        BN_ERROR("ItemPickup in invalid room=", (int)room);
    }

    ctx.game.startDialog();

    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    // Remove item after the dialog ends
    destroyIfAlreadyGot(ctx);

    co_return;
}

void ItemPickup::destroyIfAlreadyGot(GameContext& ctx)
{
    const auto room = ctx.state.getRoom();
    const auto& flags = ctx.state.getFlags();

    // TODO: Add more items
    if ((room == RoomKind::ROOM_RUINS14 && flags.got_ribbon) ||
        (room == RoomKind::ROOM_RUINS18OLD && flags.got_toyknife) ||
        (room == RoomKind::ROOM_ASRIELROOM && flags.got_bscotch_pie != GameFlags::GotBscotchPie::IN_ROOM))
        _entity.setDestroyed(true);
}

} // namespace ut::game::cpnt::inter
