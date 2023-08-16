#include "game/cpnt/inter/SavePoint.hpp"

#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "scene/Game.hpp"

#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

SavePoint::SavePoint(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<SavePoint>(), isEnabled, triggers)
{
}

auto SavePoint::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    ctx.isSavePromptRequested = true;

    asset::getSfx(asset::SfxKind::HEAL_BIG)->play();

    const int lackingHp = ctx.state.getMaxHp() - ctx.state.getCurHp();
    if (lackingHp > 0)
        ctx.state.changeHp(+lackingHp);

    ctx.msg.clear();
    const auto room = ctx.state.getRoom();
    switch (room)
    {
        // TODO: Add flag conditions, currently comment out
        using namespace ut::asset;

    case RoomKind::ROOM_CASTLE_FRONT:
    case RoomKind::ROOM_CASTLE_THRONEROOM:
    case RoomKind::ROOM_CASTLE_FINALSHOEHORN:
    case RoomKind::ROOM_CASTLE_PREBARRIER:
    case RoomKind::ROOM_SANSCORRIDOR:
    case RoomKind::ROOM_CASTLE_ELEVATOROUT:
    case RoomKind::ROOM_TRUELAB_HUB:
    case RoomKind::ROOM_TRUELAB_BEDROOM:
        // Show save prompt directly
        ctx.game.openSavePrompt();
        co_return;

    case RoomKind::ROOM_RUINS1:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_127));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_128));
        break;
    case RoomKind::ROOM_RUINS7:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_129));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_130));
        break;
    case RoomKind::ROOM_RUINS12A:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_131));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_132));
        break;
    case RoomKind::ROOM_RUINS19:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_133));
        break;
    case RoomKind::ROOM_TUNDRA1:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_135));
        break;
    case RoomKind::ROOM_TUNDRA3:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_136));
        break;
    case RoomKind::ROOM_TUNDRA_SPAGHETTI:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_137));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_138));
        break;
    case RoomKind::ROOM_TUNDRA_LESSERDOG:
        if (false) // if (`status_lesserdog` (global.flag[55]))
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_142));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_143));
        }
        else
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_140));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_141));
        }
        break;
    case RoomKind::ROOM_TUNDRA_TOWN:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_144));
        break;
    case RoomKind::ROOM_WATER2:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_145));
        break;
    case RoomKind::ROOM_WATER4:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_146));
        break;
    case RoomKind::ROOM_WATER_SAVEPOINT1:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_147));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_148));
        break;
    case RoomKind::ROOM_WATER_PREUNDYNE:
        if (false) // if (`music_statue_on` (global.flag[86]))
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_151));
        else
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_150));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_152));
        break;
    case RoomKind::ROOM_WATER_TRASHZONE2:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_153));
        break;
    case RoomKind::ROOM_WATER_TRASHSAVEPOINT:
        if (false) // if (`interacted_garbage_savepoint` (global.flag[91]))
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_161));
        else
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_156));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_157));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_158));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_159));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_160));
        }
        // TODO: Set `interacted_garbage_savepoint` (global.flag[91]) to `true`
        break;
    case RoomKind::ROOM_WATER_FRIENDLYHUB:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_164));
        break;
    case RoomKind::ROOM_WATER_TEMVILLAGE:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_165));
        break;
    case RoomKind::ROOM_WATER_UNDYNEFINAL:
        if (false) // if (`status_undyne` == KILLED (global.flag[350] == 1))
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_170));
        else if (false) // if (`undyne_difficulty` (global.flag[99]) > 0)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_169));
        else
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_168));
        break;
    case RoomKind::ROOM_FIRE_PRELAB:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_174));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_175));
        break;
    case RoomKind::ROOM_FIRE6:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_180));
        break;
    case RoomKind::ROOM_FIRE_SAVEPOINT1:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_184));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_185));
        break;
    case RoomKind::ROOM_FIRE_MEWMEW2:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_187));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_188));
        break;
    case RoomKind::ROOM_FIRE_HOTELFRONT_1:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_191));
        break;
    case RoomKind::ROOM_FIRE_HOTELLOBBY:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_195));
        break;
    case RoomKind::ROOM_FIRE_CORE_BRANCH:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_199));
        break;
    case RoomKind::ROOM_FIRE_CORE_PREMETT:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_203));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_204));
        break;
    case RoomKind::ROOM_FIRE_SAVEPOINT2:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_208));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_209));
        break;

    default:
        BN_ERROR("Save point in invalid room=", (int)room);
    }

    // TODO: Add genocide save point dialogs
    ctx.game.startDialog();

    co_return;
}

} // namespace ut::game::cpnt::inter
