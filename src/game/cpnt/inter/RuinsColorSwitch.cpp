#include "game/cpnt/inter/RuinsColorSwitch.hpp"

#include "core/ChoiceMsgKind.hpp"
#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "scene/Game.hpp"

#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

RuinsColorSwitch::RuinsColorSwitch(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<RuinsColorSwitch>(), isEnabled, triggers)
{
}

auto RuinsColorSwitch::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    const auto& flags = ctx.state.getFlags();

    ctx.msg.clear();
    ctx.msg.push_back(asset::gen::getTextEn(asset::gen::TextData::SCR_TEXT_1778));

    const auto room = ctx.state.getRoom();

    using CMKind = core::ChoiceMsgKind;

    if (room == RoomKind::ROOM_RUINS15A)
    {
        ctx.leftChoiceMsg = CMKind::PRESS_COLOR_SWITCH_NOTHING_HAPPENED;
        ctx.rightChoiceMsg =
            (flags.ruins_switches_pressed > 25) ? CMKind::PRESS_COLOR_SWITCH_TOO_MANY_TIMES : CMKind::CLOSE_IMMEDIATELY;
    }
    else if (room == RoomKind::ROOM_RUINS15B || room == RoomKind::ROOM_RUINS15C || room == RoomKind::ROOM_RUINS15D)
    {
        ctx.leftChoiceMsg = CMKind::PRESS_COLOR_SWITCH_OPEN;
        ctx.rightChoiceMsg = CMKind::CLOSE_IMMEDIATELY;
    }
    else
        BN_ERROR("RuinsColorSwitch in invalid room=", (int)room);

    ctx.game.startDialog();

    co_return;
}

} // namespace ut::game::cpnt::inter
