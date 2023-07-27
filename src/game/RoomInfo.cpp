#include "game/RoomInfo.hpp"

#include <bn_string_view.h>

#include "gen/TextData.hpp"

namespace ut::game
{

auto getRoomName(RoomKind kind) -> const bn::string_view&
{
    using namespace asset::gen;

    switch (kind)
    {
    case RoomKind::ROOM_RUINS1:
        return getTextEn(TextData::roomname_ruins1);
    case RoomKind::ROOM_RUINS7:
        return getTextEn(TextData::roomname_ruins7);
    case RoomKind::ROOM_RUINS12A:
        return getTextEn(TextData::roomname_ruins12A);
    case RoomKind::ROOM_RUINS19:
        return getTextEn(TextData::roomname_ruins19);
    case RoomKind::ROOM_TUNDRA1:
        return getTextEn(TextData::roomname_tundra1);
    case RoomKind::ROOM_TUNDRA3:
        return getTextEn(TextData::roomname_tundra3);
    case RoomKind::ROOM_TUNDRA_SPAGHETTI:
        return getTextEn(TextData::roomname_tundra_spaghetti);
    case RoomKind::ROOM_TUNDRA_LESSERDOG:
        return getTextEn(TextData::roomname_tundra_lesserdog);
    case RoomKind::ROOM_TUNDRA_TOWN:
        return getTextEn(TextData::roomname_tundra_town);
    case RoomKind::ROOM_WATER2:
        return getTextEn(TextData::roomname_water2);
    case RoomKind::ROOM_WATER4:
        return getTextEn(TextData::roomname_water4);
    case RoomKind::ROOM_WATER_SAVEPOINT1:
        return getTextEn(TextData::roomname_water_savepoint1);
    case RoomKind::ROOM_WATER_PREUNDYNE:
        return getTextEn(TextData::roomname_water_preundyne);
    case RoomKind::ROOM_WATER_TRASHSAVEPOINT:
        return getTextEn(TextData::roomname_water_trashsavepoint);
    case RoomKind::ROOM_WATER_FRIENDLYHUB:
        return getTextEn(TextData::roomname_water_friendlyhub);
    case RoomKind::ROOM_WATER_TEMVILLAGE:
        return getTextEn(TextData::roomname_water_temvillage);
    case RoomKind::ROOM_WATER_UNDYNEFINAL:
        return getTextEn(TextData::roomname_water_undynefinal);
    case RoomKind::ROOM_FIRE_MEWMEW2:
        return getTextEn(TextData::roomname_fire_mewmew2);
    case RoomKind::ROOM_FIRE_PRELAB:
        return getTextEn(TextData::roomname_fire_prelab);
    case RoomKind::ROOM_FIRE6:
        return getTextEn(TextData::roomname_fire6);
    case RoomKind::ROOM_FIRE_SAVEPOINT1:
        return getTextEn(TextData::roomname_fire_savepoint1);
    case RoomKind::ROOM_FIRE_SAVEPOINT2:
        return getTextEn(TextData::roomname_fire_savepoint2);
    case RoomKind::ROOM_FIRE_HOTELLOBBY:
        return getTextEn(TextData::roomname_fire_hotellobby);
    case RoomKind::ROOM_FIRE_CORE_BRANCH:
        return getTextEn(TextData::roomname_fire_core_branch);
    case RoomKind::ROOM_FIRE_CORE_PREMETT:
        return getTextEn(TextData::roomname_fire_core_premett);
    case RoomKind::ROOM_CASTLE_ELEVATOROUT:
        return getTextEn(TextData::roomname_castle_elevatorout);
    case RoomKind::ROOM_CASTLE_FRONT:
        return getTextEn(TextData::roomname_castle_front);
    case RoomKind::ROOM_SANSCORRIDOR:
        return getTextEn(TextData::roomname_sanscorridor);
    case RoomKind::ROOM_CASTLE_FINALSHOEHORN:
        return getTextEn(TextData::roomname_castle_finalshoehorn);
    case RoomKind::ROOM_CASTLE_THRONEROOM:
        return getTextEn(TextData::roomname_castle_throneroom);
    case RoomKind::ROOM_CASTLE_PREBARRIER:
        return getTextEn(TextData::roomname_castle_prebarrier);
    case RoomKind::ROOM_TRUELAB_HUB:
        return getTextEn(TextData::roomname_truelab_hub);
    case RoomKind::ROOM_TRUELAB_BEDROOM:
        return getTextEn(TextData::roomname_truelab_bedroom);

    default:
        break;
    }

    return getTextEn(TextData::roomname_0);
}

} // namespace ut::game
