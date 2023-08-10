#include "game/cpnt/inter/Readable.hpp"

#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/cpnt/Sprite.hpp"
#include "scene/Game.hpp"

#include "core/ChoiceMsgKind.hpp"
#include "gen/EntityId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

Readable::Readable(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<Readable>(), isEnabled, triggers)
{
    // TODO: Destroy certain readables when condition is met
}

void Readable::awake(GameContext& ctx)
{
    using EntityId = ent::gen::EntityId;

    if (_entity.getId() == EntityId::candy_dish)
    {
        const auto& flags = ctx.state.getFlags();
        if (flags.candy_taken >= 4)
            dropCandyDish(ctx);
    }
}

void Readable::dropCandyDish(GameContext& ctx)
{
    using EntityId = ent::gen::EntityId;
    BN_ASSERT(_entity.getId() == EntityId::candy_dish, "dropCandyDish() called for non `candy_dish`");

    auto* dishSpr = _entity.getComponent<Sprite>();
    BN_ASSERT(dishSpr != nullptr);

    auto* candy = ctx.entMngr.findById(EntityId::candy);
    BN_ASSERT(candy != nullptr);
    auto* candySpr = candy->getComponent<Sprite>();
    BN_ASSERT(candySpr != nullptr);

    constexpr int CANDY_DISH_DROPPED_GFXIDX = 1;
    dishSpr->setGfxIdx(CANDY_DISH_DROPPED_GFXIDX);
    candySpr->setEnabled(true);
}

void Readable::onInteract(GameContext& ctx)
{
    Interaction::onInteract(ctx);

    auto& flags = ctx.state.getFlags();

    ctx.msg.clear();
    const auto room = ctx.state.getRoom();
    switch (room)
    {
        // TODO: Add conditions, currently comment out (`EntityId`s)
        // TODO: Add readable message cases out of ruins
        using namespace ut::asset;

    case RoomKind::ROOM_AREA1:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1535));
        break;
    case RoomKind::ROOM_RUINS2:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_65));
        break;
    case RoomKind::ROOM_RUINS3:
        if (_entity.getId() == ent::gen::EntityId::sign)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1540));
        else // wall post
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_66));
        break;
    case RoomKind::ROOM_RUINS5:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_67));
        break;
    case RoomKind::ROOM_RUINS7A:
        if (flags.candy_taken >= 4)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1606));
        else
        {
            ctx.leftChoiceMsg = core::ChoiceMsgKind::TAKE_CANDY_YES;
            ctx.rightChoiceMsg = core::ChoiceMsgKind::TAKE_CANDY_NO;

            if (flags.candy_taken == 0)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1604));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1603));

            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1607));
        }
        break;
    case RoomKind::ROOM_RUINS9:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_68));
        break;
    case RoomKind::ROOM_RUINS10:
        if (_entity.getId() == ent::gen::EntityId::up)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1552));
        else if (_entity.getId() == ent::gen::EntityId::down)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1548));
        else
            BN_ERROR("Invalid readable in `room_ruins10`");
        break;
    case RoomKind::ROOM_RUINS12B:
    case RoomKind::ROOM_FIRE_SPIDERSHOP:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_sign_room_66));
        break;
    case RoomKind::ROOM_RUINS13:
        if (_entity.getId() == ent::gen::EntityId::sign)
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_sign2_55));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_sign2_56));
        }
        else // ant froggit
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_69_console_1));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_69_console_2));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_69_console_3));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_69_console_4));
        }
        break;
    case RoomKind::ROOM_RUINS14:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_70));
        break;
    case RoomKind::ROOM_RUINS15A:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_71));
        break;
    case RoomKind::ROOM_RUINS15B:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_72));
        break;
    case RoomKind::ROOM_RUINS15C:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_73));
        break;
    case RoomKind::ROOM_RUINS15D:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_74));
        break;
    case RoomKind::ROOM_TORHOUSE1:
        if (_entity.getId() == ent::gen::EntityId::books)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_75));
        else if (_entity.getId() == ent::gen::EntityId::calendar)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_114));
        else
            BN_ERROR("Invalid readable in `room_torhouse1`");
        break;
    case RoomKind::ROOM_ASGHOUSE1:
        // TODO: Add tilemap `EntityId`s on `room_asghouse1.tmx`
        if (false) // if (_entity.getId() == ent::gen::EntityId::calendar)
        {
            if (false) // if (murderlv >= 16)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_79));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_78));
        }
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::flower)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_115));
        else
            BN_ERROR("Invalid readable in `room_asghouse1`");
        break;
    case RoomKind::ROOM_TORHOUSE2:
        if (_entity.getId() == ent::gen::EntityId::home_tools)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_81));
        else if (_entity.getId() == ent::gen::EntityId::history_book)
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_80));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_81));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_82));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_83));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_84));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_85));
        }
        else
            BN_ERROR("Invalid readable in `room_torhouse2`");
        break;
    case RoomKind::ROOM_ASGHOUSE2:
        // TODO: Add tilemap `EntityId`s on `room_asghouse2.tmx`
        if (false) // if (_entity.getId() == ent::gen::EntityId::books)
        {
            if (false) // if (murderlv >= 16)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_105));
            else
            {
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_100));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_101));
            }
        }
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::home_tools)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_82));
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::fire)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room3_79));
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::flower)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_67));
        else
            BN_ERROR("Invalid readable in `room_asghouse2`");
        break;
    case RoomKind::ROOM_ASRIELROOM:
        if (_entity.getId() == ent::gen::EntityId::toys)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_83));
        else if (_entity.getId() == ent::gen::EntityId::photo)
        {
            if (flags.true_pacifist)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_67));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_63));
        }
        else if (_entity.getId() == ent::gen::EntityId::shoes)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room3_60));
        else
            BN_ERROR("Invalid readable in `room_asrielroom`");
        break;
    case RoomKind::ROOM_ASRIELROOM_FINAL:
        // TODO: Add tilemap `EntityId`s on `room_asrielroom_final.tmx`
        if (false) // if (_entity.getId() == ent::gen::EntityId::toys)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_84));
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::photo)
        {
            if (false) // if (murdurlv >= 16)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_75));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_72));
        }
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::drawing)
        {
            if (false) // if (murdurlv >= 16)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room3_66));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room3_63));
        }
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::chara_bed)
        {
            if (false) // if (murdurlv >= 16)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_99));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_98));
        }
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::asriel_bed)
        {
            if (false) // if (murdurlv >= 16)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_105));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_104));
        }
        else
            BN_ERROR("Invalid readable in `room_asrielroom_final`");
        break;
    case RoomKind::ROOM_CASTLE_FINALSHOEHORN:
        ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_86));
        break;
    case RoomKind::ROOM_CASTLE_COFFINS2:
        if (false) // if (murderlv >= 16)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_103));
        else if (flags.true_pacifist)
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_96));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_97));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_98));
        }
        else
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_90));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_91));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_92));
        }
        break;
    case RoomKind::ROOM_TORIELROOM:
        if (_entity.getId() == ent::gen::EntityId::bucket)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_124));
        else if (_entity.getId() == ent::gen::EntityId::diary)
        {
            ctx.leftChoiceMsg = core::ChoiceMsgKind::TORIEL_DIARY_YES;
            ctx.rightChoiceMsg = core::ChoiceMsgKind::CLOSE_IMMEDIATELY;
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1824));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1825));
        }
        else if (_entity.getId() == ent::gen::EntityId::socks)
        {
            if (flags.true_pacifist)
            {
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_118));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_119));
            }
            else if (_readCount > 0)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_114));
            else
            {
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_112));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_113));
            }
        }
        else if (_entity.getId() == ent::gen::EntityId::books)
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_127));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_128));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_129));
            flags.know_water_sausage = true;
        }
        else if (_entity.getId() == ent::gen::EntityId::bed)
        {
            if (flags.true_pacifist)
            {
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room3_74));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room3_75));
            }
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room3_71));
        }
        else
            BN_ERROR("Invalid readable in `room_torielroom`");
        break;
    case RoomKind::ROOM_ASGOREROOM:
        // TODO: Add tilemap `EntityId`s on `room_asgoreroom.tmx`
        if (false) // if (_entity.getId() == ent::gen::EntityId::home_drawer)
        {
            if (false) // if (murderlv >= 16)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_130));
            else
            {
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_125));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_126));
            }
        }
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::asgore_bureau)
        {
            if (false) // if (murderlv >= 16)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_94));
            else
            {
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_90));
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_91));
            }
        }
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::asgore_journal)
        {
            if (false) // if (murderlv >= 16)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room5_80));
            else
            {
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room5_74));
                if (flags.true_pacifist)
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room5_78));
                else
                    ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room5_75));
            }
        }
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::asgore_chair)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_134));
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::asgore_drawing)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room3_81));
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::bed)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_110));
        else
            BN_ERROR("Invalid readable in `room_asgoreroom`");
        break;
    case RoomKind::ROOM_CASTLE_THRONEROOM:
        // TODO: Add tilemap `EntityId`s on `room_castle_throneroom.tmx`
        if (false) // if (_entity.getId() == ent::gen::EntityId::throne)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_139));
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::covered_throne)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_119));
        else
            BN_ERROR("Invalid readable in `room_castle_throneroom`");
        break;
    case RoomKind::ROOM_TORHOUSE3:
        if (_entity.getId() == ent::gen::EntityId::room_under_renovations)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_142));
        else if (_entity.getId() == ent::gen::EntityId::mirror)
        {
            if (flags.kills_ruins >= 20)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_66));
            else if (flags.true_pacifist)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_64));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_63));
        }
        else if (_entity.getId() == ent::gen::EntityId::water_sausage)
        {
            if (flags.know_water_sausage)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_136));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_135));
        }
        else if (_entity.getId() == ent::gen::EntityId::home_drawer)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room3_83));
        else
            BN_ERROR("Invalid readable in `room_torhouse3`");
        break;
    case RoomKind::ROOM_ASGHOUSE3:
        // TODO: Add tilemap `EntityId`s on `room_asghouse3.tmx`
        if (false) // if (_entity.getId() == ent::gen::EntityId::room_under_renovations)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_143));
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::flower)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_111));
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::mirror)
        {
            if (false) // if (murderlv >= 16)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_73));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_70));
        }
        else
            BN_ERROR("Invalid readable in `room_asghouse3`");
        break;
    case RoomKind::ROOM_KITCHEN:
        if (_entity.getId() == ent::gen::EntityId::fridge)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_144));
        else if (_entity.getId() == ent::gen::EntityId::pie)
        {
            if (flags.true_pacifist)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_144));
            else if (flags.got_bscotch_pie)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_141));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room2_140));
        }
        else if (_entity.getId() == ent::gen::EntityId::sink)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room3_84));
        else if (_entity.getId() == ent::gen::EntityId::cupboard)
        {
            if (flags.kills_ruins >= 20)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_77));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_76));
        }
        else if (_entity.getId() == ent::gen::EntityId::stove)
        {
            if (flags.status_toriel == GameFlags::StatusToriel::KILLED)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_79));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_78));
        }
        else
            BN_ERROR("Invalid readable in `room_kitchen`");
        break;
    case RoomKind::ROOM_KITCHEN_FINAL:
        // TODO: Add tilemap `EntityId`s on `room_kitchen_final.tmx`
        if (false) // if (_entity.getId() == ent::gen::EntityId::fridge)
        {
            if (false) // if (murderlv >= 16)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_151));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room1_147));
        }
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::sink)
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room3_85));
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::cupboard)
        {
            if (false) // if (murdurlv >= 16)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_84));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_83));
        }
        else if (false) // else if (_entity.getId() == ent::gen::EntityId::stove)
        {
            if (false) // if (murdurlv >= 16)
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_91));
            else
                ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_readable_room4_90));
        }
        else
            BN_ERROR("Invalid readable in `room_kitchen_final`");
        break;

    default:
        BN_ERROR("Readable in invalid room=", (int)room);
    };

    ctx.game.startDialog();

    ++_readCount;
}

} // namespace ut::game::cpnt::inter
