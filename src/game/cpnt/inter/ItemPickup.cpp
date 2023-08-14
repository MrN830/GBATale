#include "game/cpnt/inter/ItemPickup.hpp"

#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/ent/Entity.hpp"
#include "scene/Game.hpp"

#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

ItemPickup::ItemPickup(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<ItemPickup>(), isEnabled, triggers)
{
}

void ItemPickup::awake(GameContext& ctx)
{
    destroyIfAlreadyGot(ctx);
}

void ItemPickup::onInteract(GameContext& ctx)
{
    Interaction::onInteract(ctx);

    auto& state = ctx.state;

    auto& flags = state.getFlags();
    auto& items = state.getItems();
    const auto room = state.getRoom();

    ctx.msg.clear();

    // TODO: Add more items
    switch (room)
    {
        using namespace ut::asset;

    case RoomKind::ROOM_RUINS14:
        if (items.full())
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1772));
        else
        {
            items.push_back(ItemKind::FADED_RIBBON);
            flags.got_ribbon = true;

            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1771));
        }
        break;

    case RoomKind::ROOM_RUINS18OLD:
        if (items.full())
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1818));
        else
        {
            items.push_back(ItemKind::TOY_KNIFE);
            flags.got_toyknife = true;

            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1817));
        }
        break;

    case RoomKind::ROOM_ASRIELROOM:
        if (items.full())
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1843));
        else
        {
            items.push_back(flags.hardmode ? ItemKind::SNAIL_PIE : ItemKind::BUTTERSCOTCH_PIE);
            flags.got_bscotch_pie = GameFlags::GotBscotchPie::GOT;

            if (flags.hardmode)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1853));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1842));
        }
        break;

    default:
        BN_ERROR("ItemPickup in invalid room=", (int)room);
    }

    // TODO: Remove item after the dialog ends
    destroyIfAlreadyGot(ctx);

    ctx.game.startDialog();
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
