#include "game/cpnt/inter/RuinsTorielCall.hpp"

#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "core/DialogChoice.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/ent/Entity.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

RuinsTorielCall::RuinsTorielCall(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<RuinsTorielCall>(), isEnabled, triggers)
{
}

void RuinsTorielCall::awake(GameContext& ctx)
{
    using EntityId = ent::gen::EntityId;

    const auto room = ctx.state.getRoom();
    const auto plot = ctx.state.getPlot();
    const auto entityId = _entity.getId();

    if ((room == RoomKind::ROOM_RUINS7 && plot >= GamePlot::RECEIVED_TORIEL_PUZZLE_CALL) ||
        (room == RoomKind::ROOM_RUINS9 &&
         ((entityId == EntityId::left && plot >= GamePlot::RECEIVED_TORIEL_PREFERENCE_CALL) ||
          (entityId == EntityId::mid && plot >= GamePlot::RECEIVED_TORIEL_CONFIRMATION_CALL) ||
          (entityId == EntityId::right && plot >= GamePlot::RECEIVED_TORIEL_ALLERGIES_CALL))) ||
        (room == RoomKind::ROOM_RUINS13 && plot >= GamePlot::RECEIVED_TORIEL_POCKETS_CALL))
    {
        _entity.setDestroyed(true);
    }
}

auto RuinsTorielCall::onInteract(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    Interaction::onInteract(ctx);

    task::DialogChoiceAwaiter dialogChoiceAwaiter;
    task::SignalAwaiter dialogEndAwaiter({task::TaskSignal::Kind::DIALOG_END});

    auto& flags = ctx.state.getFlags();
    const auto room = ctx.state.getRoom();

    getSfx(SfxKind::PHONE)->play();

    ctx.msg.clear();
    switch (room)
    {
    case RoomKind::ROOM_RUINS7:
        ctx.state.setPlot(GamePlot::RECEIVED_TORIEL_PUZZLE_CALL);

        if (flags.dog_call_status == GameFlags::DogCallStatus::DOG_KIDNAPPED_PHONE)
        {
            flags.dog_call_status = GameFlags::DogCallStatus::TORIEL_RECOVERED_PHONE;

            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_102));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_103));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_104));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_105));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_106));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_107));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_108));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_109));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_110));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_111));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_112));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_113));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_114));
        }
        else
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_90));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_91));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_92));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_93));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_94));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_95));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_96));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_97));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall1_98));
        }
        break;
    case RoomKind::ROOM_RUINS9: {
        using EntityId = ent::gen::EntityId;
        const auto entityId = _entity.getId();
        if (entityId == EntityId::left)
        {
            ctx.state.setPlot(GamePlot::RECEIVED_TORIEL_PREFERENCE_CALL);

            // Toriel's pie flavor choice
            auto& persist = ctx.state.getPersistData();
            if (persist.toriel_bscotch != PersistData::TorielBscotch::INIT)
            {
                // Toriel remembers pie flavor
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5285));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5286));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5290));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5291));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5292));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5293));
                if (persist.toriel_bscotch == PersistData::TorielBscotch::BUTTERSCOTCH)
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5295));
                else if (persist.toriel_bscotch == PersistData::TorielBscotch::CINNAMON)
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5296));
                else
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5294));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5297));
                ctx.game.startDialog();
                const auto dialogChoice = co_await dialogChoiceAwaiter;

                ctx.msgSettings =
                    core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
                ctx.msg.clear();

                if (dialogChoice == core::DialogChoice::LEFT)
                {
                    if (persist.toriel_bscotch == PersistData::TorielBscotch::BUTTERSCOTCH)
                        flags.choice_flavor = GameFlags::ChoiceFlavor::BUTTERSCOTCH;
                    else
                        flags.choice_flavor = GameFlags::ChoiceFlavor::CINNAMON;

                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5309));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5310));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5311));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5312));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5313));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5314));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5315));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5316));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5317));
                }
                else
                {
                    if (persist.toriel_bscotch == PersistData::TorielBscotch::BUTTERSCOTCH)
                    {
                        flags.choice_flavor = GameFlags::ChoiceFlavor::CINNAMON;
                        persist.toriel_bscotch = PersistData::TorielBscotch::CINNAMON;
                    }
                    else
                    {
                        flags.choice_flavor = GameFlags::ChoiceFlavor::BUTTERSCOTCH;
                        persist.toriel_bscotch = PersistData::TorielBscotch::BUTTERSCOTCH;
                    }
                    ctx.state.savePersist();

                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5323));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5324));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5325));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_5326));
                }
            }
            else // Toriel doesn't remember pie flavor
            {
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_467));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_468));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_469));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_470));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_471));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_472));
                ctx.game.startDialog();
                const auto dialogChoice = co_await dialogChoiceAwaiter;

                if (dialogChoice == core::DialogChoice::RIGHT)
                {
                    flags.choice_flavor = GameFlags::ChoiceFlavor::BUTTERSCOTCH;
                    persist.toriel_bscotch = PersistData::TorielBscotch::BUTTERSCOTCH;
                }
                else
                {
                    flags.choice_flavor = GameFlags::ChoiceFlavor::CINNAMON;
                    persist.toriel_bscotch = PersistData::TorielBscotch::CINNAMON;
                }
                ctx.state.savePersist();

                ctx.msgSettings =
                    core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
                ctx.msg.clear();
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_490));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_491));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_492));
            }
        }
        else if (entityId == EntityId::mid)
        {
            ctx.state.setPlot(GamePlot::RECEIVED_TORIEL_CONFIRMATION_CALL);

            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall3_88));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall3_89));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall3_90));
            if (flags.choice_flavor == GameFlags::ChoiceFlavor::BUTTERSCOTCH)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall3_92));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall3_91));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall3_93));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall3_94));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall3_95));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall3_96));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall3_97));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall3_98));
        }
        else if (entityId == EntityId::right)
        {
            ctx.state.setPlot(GamePlot::RECEIVED_TORIEL_ALLERGIES_CALL);

            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall4_88));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall4_89));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall4_90));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall4_91));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall4_92));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall4_93));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall4_94));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall4_95));
        }
        else
            BN_ERROR("Invalid entityId=", (int)entityId, " for `ruins9` toriel call");
        break;
    }
    case RoomKind::ROOM_RUINS13:
        ctx.state.setPlot(GamePlot::RECEIVED_TORIEL_POCKETS_CALL);
        ctx.entMngr.createStalkerFlowey({48, 128});

        if (flags.hardmode)
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_104));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_105));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_106));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_107));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_108));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_109));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_110));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_111));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_112));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_113));
        }
        else
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_90));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_91));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_92));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_93));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_94));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_95));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_96));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_97));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_98));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_99));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torielcall5_100));
        }
        break;
    default:
        BN_ERROR("RuinsTorielCall in invalid room=", (int)room);
    }

    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    ctx.msgSettings.reset();

    _entity.setDestroyed(true);
    co_return;
}

} // namespace ut::game::cpnt::inter
