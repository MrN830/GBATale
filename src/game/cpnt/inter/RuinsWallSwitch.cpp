#include "game/cpnt/inter/RuinsWallSwitch.hpp"

#include <bn_sound_item.h>

#include "asset/Path.hpp"
#include "asset/SfxKind.hpp"
#include "core/DialogSettings.hpp"
#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/cpnt/inter/TalkTorielRuins3.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/CameraManager.hpp"
#include "game/sys/EntityManager.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

RuinsWallSwitch::RuinsWallSwitch(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : RuinsSpikeDisabler(entity, bn::type_id<RuinsWallSwitch>(), isEnabled, triggers)
{
}

void RuinsWallSwitch::awake(GameContext& ctx)
{
    using EntityId = ent::gen::EntityId;

    const auto entityId = _entity.getId();
    const auto plot = ctx.state.getPlot();
    const auto room = ctx.state.getRoom();

    if ((room == RoomKind::ROOM_RUINS3 &&
         ((entityId == EntityId::button && plot >= GamePlot::FIRST_SWITCH_FLIPPED) ||
          (entityId == EntityId::button2 && plot >= GamePlot::SWITCH_PUZZLE_COMPLETE))) ||
        (room == RoomKind::ROOM_RUINS14 && plot >= GamePlot::JUST_ONE_SWITCH_PUZZLE_SOLVED))
    {
        setPressed(true);
    }
}

auto RuinsWallSwitch::onInteract(GameContext& ctx) -> task::Task
{
    RuinsSpikeDisabler::onInteract(ctx);

    if (_isPressed)
        co_return;

    setPressed(true);

    using namespace ut::asset;
    using EntityId = ent::gen::EntityId;

    const auto entityId = _entity.getId();
    const auto room = ctx.state.getRoom();

    if (room == RoomKind::ROOM_RUINS3)
    {
        // checks `nullptr` before actual use
        auto* toriel = ctx.entMngr.findById(ent::gen::EntityId::toriel);
        auto* torielWalk = (toriel) ? toriel->getComponent<WalkAnimCtrl>() : nullptr;
        auto* torielInput = (toriel) ? toriel->getComponent<NpcInput>() : nullptr;
        auto* torielInteract = (toriel) ? toriel->getComponent<inter::Interaction>() : nullptr;

        auto* plotWall = ctx.entMngr.findById(ent::gen::EntityId::plot_wall);
        BN_ASSERT(plotWall);
        auto* plotWallColl = plotWall->getComponent<ColliderPack>();
        BN_ASSERT(plotWallColl);

        task::SignalAwaiter torielWalkAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});
        task::SignalAwaiter dialogEndAwaiter({task::TaskSignal::Kind::DIALOG_END});

        if (entityId == EntityId::button)
        {
            BN_ASSERT(ctx.state.getPlot() <= GamePlot::TORIEL_WAITING_AT_SWITCH_PUZZLE);
            ctx.state.setPlot(GamePlot::FIRST_SWITCH_FLIPPED);

            plotWallColl->setEnabled(false);

            BN_ASSERT(toriel);
            BN_ASSERT(torielInput);
            BN_ASSERT(torielInteract);
            BN_ASSERT(torielInteract->getInteractionType() == bn::type_id<TalkTorielRuins3>());
            auto& torielTalk = static_cast<TalkTorielRuins3&>(*torielInteract);

            torielTalk.setTalked(false);

            // toriel walks to final pos, staring from current pos
            const bn::array<bn::fixed_point, 2> curPathPoints = {
                toriel->getPosition(), IPath::get(gen::PathId::path_torielwalk3_2).getPoints().back()};
            const Path<2> curPath(curPathPoints);
            torielInput->startPath(curPath, 2.4);
            co_await torielWalkAwaiter;

            BN_ASSERT(torielWalk);
            torielWalk->setStandStillDir(core::Directions::LEFT);

            torielTalk.setEnabled(true);
        }
        else if (entityId == EntityId::button2)
        {
            ctx.interactStack.push(InteractState::CUTSCENE);

            BN_ASSERT(torielInteract);
            torielInteract->setEnabled(false);

            hideAllSpikesInRoom(ctx);
            BN_ASSERT(ctx.state.getPlot() == GamePlot::FIRST_SWITCH_FLIPPED);
            ctx.state.setPlot(GamePlot::SWITCH_PUZZLE_COMPLETE);

            getSfx(SfxKind::SCREEN_SHAKE)->play();
            ctx.camMngr.startShake({4, 0}, 2);
            co_await task::SignalAwaiter({task::TaskSignal::Kind::CAM_SHAKE_END});

            // Toriel talks
            ctx.msg.getSettings() =
                core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
            ctx.msg.clearMsg();
            ctx.msg.add(gen::TextData::SCR_TEXT_362);
            ctx.msg.add(gen::TextData::SCR_TEXT_363);
            ctx.game.startDialog();
            co_await dialogEndAwaiter;

            ctx.msg.getSettings().reset();
            BN_ASSERT(ctx.interactStack.top() == InteractState::CUTSCENE);
            ctx.interactStack.pop();

            // Toriel goes out of the room
            cmd::MoveCmd moveCmd;
            moveCmd.directions = core::Directions::RIGHT;
            moveCmd.movePos = {2.4, 0};
            moveCmd.checkCollision = false;
            BN_ASSERT(torielInput);
            torielInput->startOneWay(moveCmd, 2 * 30);
            co_await torielWalkAwaiter;

            BN_ASSERT(toriel);
            toriel->setDestroyed(true);
        }
        else if (entityId == EntityId::button3)
        {
            setPressed(false);

            ctx.msg.clearMsg();
            if (ctx.state.getPlot() >= GamePlot::SWITCH_PUZZLE_COMPLETE)
            {
                ctx.msg.add(gen::TextData::obj_readable_switch1_91);
            }
            else if (torielInput && !torielInput->isDone())
            {
                getSfx(SfxKind::WRONG_VICTORY)->play();
                ctx.msg.add(gen::TextData::obj_readable_switch1_81);
            }
            else
            {
                ctx.msg.getSettings() =
                    core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
                ctx.msg.getSettings().emotion = 1;
                if (ctx.state.getFlags().hardmode)
                {
                    ctx.msg.add(gen::TextData::obj_readable_switch1_71);
                }
                else
                {
                    ctx.msg.add(gen::TextData::obj_readable_switch1_65);
                    ctx.msg.add(gen::TextData::obj_readable_switch1_66);
                    ctx.msg.add(gen::TextData::obj_readable_switch1_67);
                }
            }
            ctx.game.startDialog();
            co_await dialogEndAwaiter;

            ctx.msg.getSettings().reset();
        }
        else
            BN_ERROR("Invalid entityId=", (int)entityId, " for `ruins3`");
    }
    else if (room == RoomKind::ROOM_RUINS14)
    {
        ctx.interactStack.push(InteractState::INTERACT);

        hideAllSpikesInRoom(ctx);
        BN_ASSERT(ctx.state.getPlot() < GamePlot::JUST_ONE_SWITCH_PUZZLE_SOLVED);
        ctx.state.setPlot(GamePlot::JUST_ONE_SWITCH_PUZZLE_SOLVED);

        getSfx(SfxKind::SCREEN_SHAKE)->play();
        ctx.camMngr.startShake({4, 0}, 2);
        co_await task::SignalAwaiter({task::TaskSignal::Kind::CAM_SHAKE_END});

        BN_ASSERT(ctx.interactStack.top() == InteractState::INTERACT);
        ctx.interactStack.pop();
    }
    else
        BN_ERROR("RuinsWallSwitch in invalid room=", (int)room);

    co_return;
}

void RuinsWallSwitch::setPressed(bool isPressed)
{
    if (_isPressed == isPressed)
        return;

    _isPressed = isPressed;

    auto* spr = _entity.getComponent<cpnt::Sprite>();
    BN_ASSERT(spr);
    spr->setGfxIdx(isPressed);
}

} // namespace ut::game::cpnt::inter
