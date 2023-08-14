#include "game/cpnt/inter/TalkFroggit.hpp"

#include "core/ChoiceMsgKind.hpp"
#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "scene/Game.hpp"
#include "scene/IngameDialog.hpp"
#include "scene/SceneStack.hpp"

#include "gen/EntityId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

TalkFroggit::TalkFroggit(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<TalkFroggit>(), isEnabled, triggers)
{
}

void TalkFroggit::awake(GameContext& ctx)
{
    _isDialogSkipAdviceFroggit =
        (ctx.state.getRoom() == RoomKind::ROOM_RUINS13 && _entity.getId() == ent::gen::EntityId::left);

    const int kills = ctx.state.getKills();
    const auto room = ctx.state.getRoom();

    if (kills > 16)
    {
        _entity.setDestroyed(true);
    }
    else if (kills > 0)
    {
        if (room == RoomKind::ROOM_RUINS7 || room == RoomKind::ROOM_RUINS17)
            _entity.setDestroyed(true);
    }
    else if (_isDialogSkipAdviceFroggit)
    {
        const auto& flags = ctx.state.getFlags();

        if (flags.dialogues_skipped > 5 && !flags.true_pacifist)
            _entity.setDestroyed(true);
    }
}

void TalkFroggit::onInteract(GameContext& ctx)
{
    Interaction::onInteract(ctx);

    const auto& flags = ctx.state.getFlags();

    ctx.msg.clear();
    const auto room = ctx.state.getRoom();
    switch (room)
    {
        using namespace ut::asset;
        using CMKind = core::ChoiceMsgKind;

    case RoomKind::ROOM_RUINS7:
        if (flags.true_pacifist)
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_79));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_80));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_81));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_82));
        }
        else if (flags.hardmode)
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_70));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_71));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_72));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_73));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_74));
        }
        else
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_61));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_62));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_63));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_64));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_65));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_66));
        }
        break;

    case RoomKind::ROOM_RUINS17:
        if (flags.hardmode)
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_105));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_106));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_107));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_108));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_109));
        }
        else if (flags.true_pacifist)
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_97));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_98));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_99));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_100));
        }
        else
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_90));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_91));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_92));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_93));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_94));
        }
        break;

    case RoomKind::ROOM_RUINS13:
        if (_entity.getId() == ent::gen::EntityId::left)
        {
            if (flags.true_pacifist)
            {
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_xadvice_67));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_xadvice_68));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_xadvice_69));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_xadvice_70));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_xadvice_71));
            }
            else
            {
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_xadvice_59));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_xadvice_60));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_xadvice_61));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_xadvice_62));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_xadvice_63));
            }
        }
        else if (_entity.getId() == ent::gen::EntityId::mid)
        {
            if (flags.true_pacifist)
            {
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_124));
            }
            else
            {
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_117_console));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_118_console));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_119_console));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_120_console));
            }
        }
        else if (_entity.getId() == ent::gen::EntityId::right)
        {
            if (flags.true_pacifist)
            {
                if (flags.name_color == GameFlags::NameColor::YELLOW)
                {
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_140));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_141));
                }
                else if (flags.name_color == GameFlags::NameColor::WHITE)
                {
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_145));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_146));
                }
                else if (flags.name_color == GameFlags::NameColor::PINK)
                {
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_150));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_smallfrog_151));
                }
                else
                    BN_ERROR("Invalid GameFlags::NameColor=", (int)flags.name_color);
            }
            else
            {
                if (flags.name_color == GameFlags::NameColor::YELLOW)
                {
                    ctx.leftChoiceMsg = CMKind::YELLOW_NAME_HELPFUL;
                    ctx.rightChoiceMsg = CMKind::YELLOW_NAME_BAD;
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4477));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4478));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4479));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4480));
                }
                else if (flags.name_color == GameFlags::NameColor::WHITE)
                {
                    ctx.leftChoiceMsg = CMKind::NO_NAME_COLOR_GREAT;
                    ctx.rightChoiceMsg = CMKind::BRING_NAME_COLOR_BACK;
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4511));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4512));
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4513));
                }
                else if (flags.name_color == GameFlags::NameColor::PINK)
                {
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4535));
                }
                else
                    BN_ERROR("Invalid GameFlags::NameColor=", (int)flags.name_color);
            }
        }
        else
            BN_ERROR("Invalid TalkFroggit in `room_ruins13`");
        break;

    default:
        BN_ERROR("TalkFroggit in invalid room=", (int)room);
    }

    if (_isDialogSkipAdviceFroggit)
    {
        _isDialogSkipAdviceStarted = true;
        _isDialogSkipAdviceChanged = false;
    }

    ctx.game.startDialog();
}

void TalkFroggit::update(GameContext& ctx)
{
    if (_isDialogSkipAdviceFroggit && _isDialogSkipAdviceStarted && !_isDialogSkipAdviceChanged)
    {
        auto& scene = ctx.game.getSceneStack().getTopScene();
        if (scene.getId() == scene::SceneId::INGAME_DIALOG)
        {
            auto& dialogScene = static_cast<scene::IngameDialog&>(scene);
            const auto& writer = dialogScene.getWriter();
            if (!writer.isStarted())
            {
                _isDialogSkipAdviceStarted = false;
                return;
            }

            const int dialogIdx = writer.getCurDialogIdx();
            const auto& flags = ctx.state.getFlags();

            if (dialogIdx <= 1)
            {
                _prevDialogSkippedCount = flags.dialogues_skipped;
            }
            else if (dialogIdx == 3)
            {
                using namespace ut::asset;

                if (_prevDialogSkippedCount < flags.dialogues_skipped)
                {
                    auto& dialogs = dialogScene.getDialogs();

                    dialogs.pop_back();

                    if (flags.true_pacifist)
                        dialogs.emplace_back(dialogs.back().settingsKind,
                                             gen::getTextEn(gen::TextData::obj_smallfrog_xadvice_153));
                    else
                        dialogs.emplace_back(dialogs.back().settingsKind,
                                             gen::getTextEn(gen::TextData::obj_smallfrog_xadvice_150));

                    _isDialogSkipAdviceChanged = true;
                }
            }
        }
    }
}

} // namespace ut::game::cpnt::inter
