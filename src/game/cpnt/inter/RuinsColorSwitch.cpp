#include "game/cpnt/inter/RuinsColorSwitch.hpp"

#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "core/DialogChoice.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "scene/Game.hpp"

#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

RuinsColorSwitch::RuinsColorSwitch(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : RuinsSpikeDisabler(entity, bn::type_id<RuinsColorSwitch>(), isEnabled, triggers)
{
}

auto RuinsColorSwitch::onInteract(GameContext& ctx) -> task::Task
{
    RuinsSpikeDisabler::onInteract(ctx);

    using namespace ut::asset;

    auto& state = ctx.state;
    auto& flags = state.getFlags();
    const auto room = state.getRoom();
    const auto plot = state.getPlot();

    ctx.msg.clear();
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1778));

    // Dialog choice: Press color switch
    ctx.game.startDialog();
    const auto dialogChoice = co_await task::DialogChoiceAwaiter();
    ctx.msg.clear();

    if (room == RoomKind::ROOM_RUINS15A)
    {
        if (dialogChoice == core::DialogChoice::LEFT)
        {
            flags.ruins_switches_pressed += 1;

            if (flags.ruins_switches_pressed > 25)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1790));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1788));
        }
        else if (dialogChoice == core::DialogChoice::RIGHT)
        {
            if (flags.ruins_switches_pressed > 25)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1790));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1789));
        }
        else
            BN_ERROR("Invalid dialog choice=", (int)dialogChoice);
    }
    else if (room == RoomKind::ROOM_RUINS15B || room == RoomKind::ROOM_RUINS15C || room == RoomKind::ROOM_RUINS15D)
    {
        if (dialogChoice == core::DialogChoice::LEFT)
        {
            using Room = game::RoomKind;
            using Plot = game::GamePlot;

            flags.ruins_switches_pressed += 1;
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1806));

            if (room == Room::ROOM_RUINS15B && (int)plot < (int)Plot::BLUE_SWITCH_FLIPPED)
            {
                state.setPlot(Plot::BLUE_SWITCH_FLIPPED);

                asset::getSfx(asset::SfxKind::SWITCH_PULL_N)->play();
                hideAllSpikesInRoom(ctx);
            }
            else if (room == Room::ROOM_RUINS15C && (int)plot < (int)Plot::RED_SWITCH_FLIPPED)
            {
                state.setPlot(Plot::RED_SWITCH_FLIPPED);

                asset::getSfx(asset::SfxKind::SWITCH_PULL_N)->play();
                hideAllSpikesInRoom(ctx);
            }
            else if (room == Room::ROOM_RUINS15D && (int)plot < (int)Plot::GREEN_SWITCH_FLIPPED)
            {
                state.setPlot(Plot::GREEN_SWITCH_FLIPPED);

                asset::getSfx(asset::SfxKind::SWITCH_PULL_N)->play();
                hideAllSpikesInRoom(ctx);
            }
        }
        else if (dialogChoice == core::DialogChoice::RIGHT)
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1807));
        }
        else
            BN_ERROR("Invalid dialog choice=", (int)dialogChoice);
    }
    else
        BN_ERROR("RuinsColorSwitch in invalid room=", (int)room);

    ctx.game.startDialog();

    co_return;
}

} // namespace ut::game::cpnt::inter
