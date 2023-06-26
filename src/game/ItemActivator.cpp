#include "game/ItemActivator.hpp"

#include <bn_format.h>
#include <bn_log.h>
#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "core/Random.hpp"
#include "game/GameState.hpp"
#include "game/ItemInfo.hpp"
#include "gen/TextData.hpp"

namespace ut::game
{

using namespace ut::asset::gen;

auto ItemActivator::use(int itemIdx, GameState& gameState, core::Random& rng)
    -> bn::vector<bn::string<consts::DIALOG_MAX_CHARS>, USE_MAX_DIALOGS>
{
    auto& items = gameState.getItems();
    BN_ASSERT(0 <= itemIdx && itemIdx < items.size(), "itemIdx=", itemIdx, " OOB (size=", items.size(), ")");
    const auto& item = ItemInfo::get(items[itemIdx]);

    bn::vector<bn::string<consts::DIALOG_MAX_CHARS>, USE_MAX_DIALOGS> result;

    if (item.type == ItemType::FOOD)
    {
        // fallback sfx
        const bn::sound_item* swallowSfx = asset::getSfx(asset::SfxKind::SWALLOW);
        asset::SfxKind delayedHealSfx = asset::SfxKind::HEAL_BIG;
        int delay = 10;

        bool isPreserveItem = false;

        int hpRecover = item.hpRecover + (gameState.getWeapon() == ItemKind::BURNT_PAN ? 4 : 0);

        result.push_back("");

        switch (item.kind)
        {
        case ItemKind::MONSTER_CANDY:
            result.back().append(getTextEn(TextData::item_use_1));
            if (!gameState.isSeriousBattle())
            {
                const int num = rng.get_int(30);
                if (num < 5)
                    result.back().append(getTextEn(TextData::item_use_1a));
                else if (num == 29)
                    result.back().append(getTextEn(TextData::item_use_1b));
            }
            break;

        case ItemKind::CROQUET_ROLL:
            if (gameState.isSeriousBattle())
                result.back().append(getTextEn(TextData::item_use_2_serious));
            else
                result.back().append(getTextEn(TextData::item_use_2));
            break;

        case ItemKind::BANDAGE:
            delay = 0;
            swallowSfx = nullptr;
            result.back().append(getTextEn(TextData::item_use_4));
            if (!gameState.isSeriousBattle())
                result.back().append(getTextEn(TextData::item_use_4a));
            break;

        case ItemKind::ROCK_CANDY:
            result.back().append(getTextEn(TextData::item_use_5));
            break;

        case ItemKind::PUMPKIN_RINGS:
            result.back().append(getTextEn(TextData::item_use_6));
            break;

        case ItemKind::SPIDER_DONUT:
            if (gameState.isInBattle() && !gameState.isSeriousBattle() && rng.get_int(10) == 9)
                result.back().append(getTextEn(TextData::item_use_7a));
            else
                result.back().append(getTextEn(TextData::item_use_7));
            // TODO: Muffet trigger
            break;

        case ItemKind::STOIC_ONION:
            result.back().append(getTextEn(TextData::item_use_8));
            if (!gameState.isSeriousBattle())
            {
                const int num = rng.get_int(20);
                if (num < 3)
                    result.back().append(getTextEn(TextData::item_use_8a));
            }
            break;

        case ItemKind::GHOST_FRUIT:
            result.back().append(getTextEn(TextData::item_use_9));
            if (!gameState.isSeriousBattle())
                delayedHealSfx = asset::SfxKind::SPOOKY;
            break;

        case ItemKind::SPIDER_CIDER:
            result.back().append(getTextEn(TextData::item_use_10));
            // TODO: Muffet trigger
            break;

        case ItemKind::BUTTERSCOTCH_PIE:
            result.back().append(getTextEn(TextData::item_use_11));
            hpRecover = bn::max(0, gameState.getMaxHp() - gameState.getCurHp());
            // TODO: Asgore trigger
            // TODO: Toriel (Lost soul) trigger
            break;

        case ItemKind::SNOWMAN_PIECE:
            result.back().append(getTextEn(TextData::item_use_16));
            break;

        case ItemKind::NICE_CREAM: {
            const int num = rng.get_int(8);
            if (num == 0)
                result.back().append(getTextEn(TextData::item_use_17a));
            else if (num == 1)
                result.back().append(getTextEn(TextData::item_use_17b));
            else if (num == 2)
                result.back().append(getTextEn(TextData::item_use_17c));
            else if (num == 3)
                result.back().append(getTextEn(TextData::item_use_17d));
            else if (num == 4)
                result.back().append(getTextEn(TextData::item_use_17e));
            else if (num == 5)
                result.back().append(getTextEn(TextData::item_use_17f));
            else if (num == 6)
                result.back().append(getTextEn(TextData::item_use_17g));
            else if (num == 7)
                result.back().append(getTextEn(TextData::item_use_17h));
            break;
        }

        case ItemKind::PUPPYDOUGH_ICECREAM:
            result.back().append(getTextEn(TextData::item_use_18));
            break;

        case ItemKind::BISICLE:
            result.back().append(getTextEn(TextData::item_use_19));
            items[itemIdx] = ItemKind::UNISICLE;
            isPreserveItem = true;
            break;

        case ItemKind::UNISICLE:
            result.back().append(getTextEn(TextData::item_use_20));
            break;

        case ItemKind::CINNAMON_BUNNY:
            result.back().append(getTextEn(TextData::item_use_21));
            break;

        case ItemKind::TEMMIE_FLAKES:
            result.back().append(getTextEn(TextData::item_use_22));
            break;

        case ItemKind::ABANDONED_QUICHE:
            result.back().append(getTextEn(TextData::item_use_23));
            break;

        case ItemKind::DOG_SALAD: {
            result.back().append(getTextEn(TextData::item_use_28));
            const int num = rng.get_int(4);
            if (num == 0)
            {
                hpRecover = 30;
                result.back().append(getTextEn(TextData::item_use_28a));
            }
            else if (num == 1)
            {
                hpRecover = 10;
                result.back().append(getTextEn(TextData::item_use_28b));
            }
            else if (num == 2)
            {
                hpRecover = 2;
                result.back().append(getTextEn(TextData::item_use_28c));
            }
            else if (num == 3)
            {
                hpRecover = 999;
                result.back().append(getTextEn(TextData::item_use_28d));
            }
            if (!gameState.isSeriousBattle())
                delayedHealSfx = asset::SfxKind::DOG_RESIDUE;
            else
                delayedHealSfx = asset::SfxKind::HEAL_SMALL;
            break;
        }

        case ItemKind::ASTRONAUT_FOOD:
            result.back().append(getTextEn(TextData::item_use_35));
            break;

        case ItemKind::INSTANT_NOODLES:
            if (gameState.isInBattle())
            {
                if (gameState.isSeriousBattle())
                {
                    hpRecover = 90;
                    result.back().append(getTextEn(TextData::item_use_36a));
                }
                else
                {
                    hpRecover = 4;

                    result.back().append(getTextEn(TextData::obj_instantnoodleitem_45));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_30));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_31));
                    // TODO: Stop the music here
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_32));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_33));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_34));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_35));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_36));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_37));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_38));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_39));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_40));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_41));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_42));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_43));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_44));
                    result.push_back(getTextEn(TextData::obj_instantnoodleitem_85));
                }
            }
            else
            {
                hpRecover = 15;
                result.back().append(getTextEn(TextData::item_use_36));
            }
            break;

        case ItemKind::CRAB_APPLE:
            result.back().append(getTextEn(TextData::item_use_37));
            break;

        case ItemKind::HOT_DOG:
            result.back().append(getTextEn(TextData::item_use_38));
            if (!gameState.isSeriousBattle())
                delayedHealSfx = asset::SfxKind::DOG_SALAD;
            else
                delayedHealSfx = asset::SfxKind::HEAL_SMALL;
            break;

        case ItemKind::HOT_CAT:
            result.back().append(getTextEn(TextData::item_use_39));
            if (!gameState.isSeriousBattle())
                delayedHealSfx = asset::SfxKind::CAT_SALAD;
            break;

        case ItemKind::GLAMBURGER:
            // TODO: Mettaton rating trigger
            result.back().append(getTextEn(TextData::item_use_40));
            delayedHealSfx = asset::SfxKind::SPARKLE1;
            break;

        case ItemKind::SEA_TEA:
            result.back().append(getTextEn(TextData::item_use_41));
            delayedHealSfx = asset::SfxKind::SPEED_UP;

            if (gameState.isInBattle())
            {
                // TODO: speed increase (excluding Muffet fight)
            }

            break;

        case ItemKind::STARFAIT:
            // TODO: Mettaton rating trigger
            result.back().append(getTextEn(TextData::item_use_42));
            break;

        case ItemKind::LEGENDARY_HERO:
            // TODO: Mettaton rating trigger
            result.back().append(getTextEn(TextData::item_use_43));
            if (!gameState.isSeriousBattle())
                delayedHealSfx = asset::SfxKind::HERO;
            if (gameState.isInBattle())
            {
                // TODO: Temporary atk +4
                result.back().append(getTextEn(TextData::item_use_43_attack));
            }
            break;

        case ItemKind::BAD_MEMORY:
            if (gameState.getCurHp() <= 2)
            {
                hpRecover = bn::max(0, gameState.getMaxHp() - gameState.getCurHp());
                delayedHealSfx = asset::SfxKind::HEAL_BIG;
                result.back().append(getTextEn(TextData::item_use_54_heal));
            }
            else
            {
                delayedHealSfx = asset::SfxKind::HURT_SMALL;
                result.back().append(getTextEn(TextData::item_use_54));
            }
            break;

        case ItemKind::DREAM:
            result.back().append(getTextEn(TextData::item_use_55));
            // TODO: Only show default text for the first time, use short one after
            // result.back().append(getTextEn(TextData::item_use_55_short));
            break;

        case ItemKind::POPATO_CHISPS:
            result.back().append(getTextEn(TextData::item_use_58));
            break;

        case ItemKind::JUNK_FOOD:
            // TODO: Mettaton rating trigger
            result.back().append(getTextEn(TextData::item_use_59));
            break;

        case ItemKind::FACE_STEAK:
            // TODO: Mettaton rating trigger
            result.back().append(getTextEn(TextData::item_use_61));
            break;

        case ItemKind::HUSH_PUPPY:
            if (!gameState.isSeriousBattle())
                result.back().append(getTextEn(TextData::item_use_62a));
            else
                result.back().append(getTextEn(TextData::item_use_62));
            // TODO: Endogeny mercy max out
            break;

        case ItemKind::SNAIL_PIE:
            result.back().append(getTextEn(TextData::item_use_63));
            result.back().append("%");
            hpRecover = bn::max(0, -1 + gameState.getMaxHp() - gameState.getCurHp());
            // TODO: Asgore trigger
            // TODO: Toriel (Lost soul) trigger
            break;

        default:
            BN_ERROR("non-food item.kind=", (int)item.kind, " (type=", (int)item.type, ")");
        }

        gameState.changeHp(hpRecover);

        result.back().append(" &");

        const bool isMaxedOut = (gameState.getCurHp() >= gameState.getMaxHp());
        if (isMaxedOut)
            result.back().append(getTextEn(TextData::recover_hp_max));
        else
            result.back().append(bn::format<consts::DIALOG_MAX_CHARS>(R"(* You recovered {} HP!/%)", hpRecover));

        // play sfx
        if (swallowSfx != nullptr)
            swallowSfx->play();
        if (delayedHealSfx != asset::SfxKind::NONE)
            asset::playSfxWithDelay(delayedHealSfx, delay);

        if (!isPreserveItem)
            items.erase(items.begin() + itemIdx);
    }
    else if (item.type == ItemType::WEAPON)
    {
        asset::getSfx(asset::SfxKind::ITEM)->play();

        items.erase(items.begin() + itemIdx);
        items.push_back(gameState.getWeapon());
        gameState.setWeapon(item.kind);

        switch (item.kind)
        {
        case ItemKind::TOY_KNIFE:
            result.push_back(getTextEn(TextData::item_use_13));
            break;
        case ItemKind::TOUGH_GLOVE:
            result.push_back(getTextEn(TextData::item_use_14));
            break;
        case ItemKind::BALLET_SHOES:
            result.push_back(getTextEn(TextData::item_use_25));
            break;
        case ItemKind::TORN_NOTEBOOK:
            result.push_back(getTextEn(TextData::item_use_45));
            break;
        case ItemKind::BURNT_PAN:
            result.push_back(getTextEn(TextData::item_use_47));
            break;
        case ItemKind::EMPTY_GUN:
            result.push_back(getTextEn(TextData::item_use_49));
            break;
        case ItemKind::WORN_DAGGER:
            result.push_back(getTextEn(TextData::item_use_51));
            break;
        case ItemKind::REAL_KNIFE:
            result.push_back(getTextEn(TextData::item_use_52));
            break;
        default:
            BN_ERROR("non-weapon item.kind=", (int)item.kind, " (type=", (int)item.type, ")");
        }
    }
    else if (item.type == ItemType::ARMOR)
    {
        asset::getSfx(asset::SfxKind::ITEM)->play();

        items.erase(items.begin() + itemIdx);
        items.push_back(gameState.getArmor());
        gameState.setArmor(item.kind);

        switch (item.kind)
        {
        case ItemKind::FADED_RIBBON:
            result.push_back(getTextEn(TextData::item_use_12));
            break;
        case ItemKind::MANLY_BANDANNA:
            result.push_back(getTextEn(TextData::item_use_15));
            break;
        case ItemKind::OLD_TUTU:
            result.push_back(getTextEn(TextData::item_use_24));
            break;
        case ItemKind::CLOUDY_GLASSES:
            result.push_back(getTextEn(TextData::item_use_44));
            break;
        case ItemKind::STAINED_APRON:
            result.push_back(getTextEn(TextData::item_use_46));
            break;
        case ItemKind::COWBOY_HAT:
            result.push_back(getTextEn(TextData::item_use_48));
            break;
        case ItemKind::HEART_LOCKET:
            result.push_back(getTextEn(TextData::item_use_50));
            break;
        case ItemKind::THE_LOCKET:
            result.push_back(getTextEn(TextData::item_use_53));
            break;
        case ItemKind::TEMY_ARMOR:
            result.push_back(getTextEn(TextData::item_use_64));
            break;
        default:
            BN_ERROR("non-armor item.kind=", (int)item.kind, " (type=", (int)item.type, ")");
        }
    }
    else if (item.type == ItemType::SPECIAL)
    {
        // TODO: Implement special item use
        BN_LOG("ItemActivator::use() - ItemType::Special handling not implemented");

        switch (item.kind)
        {
        case ItemKind::STICK:
            result.push_back(getTextEn(TextData::item_use_3));
            // TODO
            break;

        case ItemKind::PUNCH_CARD:
            result.push_back(getTextEn(TextData::item_use_26));
            // TODO
            break;

        case ItemKind::ANNOYING_DOG:
            result.push_back(getTextEn(TextData::item_use_27));
            // TODO
            break;

        case ItemKind::DOG_RESIDUE_1:
        case ItemKind::DOG_RESIDUE_2:
        case ItemKind::DOG_RESIDUE_3:
        case ItemKind::DOG_RESIDUE_4:
        case ItemKind::DOG_RESIDUE_5:
        case ItemKind::DOG_RESIDUE_6:
            asset::getSfx(asset::SfxKind::ITEM)->play();
            asset::playSfxWithDelay(asset::SfxKind::DOG_RESIDUE, 15);

            result.push_back(getTextEn(TextData::item_use_29));
            if (!items.full())
                result.push_back(getTextEn(TextData::item_use_29a));
            else
                result.push_back(getTextEn(TextData::item_use_29b));
            result.push_back(getTextEn(TextData::item_use_29c));
            result.push_back(getTextEn(TextData::item_use_29d));

            items.erase(items.begin() + itemIdx);

            while (!items.full())
                items.push_back((ItemKind)((int)ItemKind::DOG_SALAD + rng.get_int(7)));
            break;

        case ItemKind::UNDYNES_LETTER:
        case ItemKind::UNDYNES_LETTER_EX:
            result.push_back(getTextEn(TextData::item_use_56));
            // TODO
            break;

        case ItemKind::MYSTERY_KEY:
            result.push_back(getTextEn(TextData::item_use_60));
            // TODO
            break;

        case ItemKind::NONE:
            BN_ERROR("Tried to use ItemKind::NONE");
            break;
        default:
            BN_ERROR("non-special item.kind=", (int)item.kind, " (type=", (int)item.type, ")");
        }
    }
    else
        BN_ERROR("item.kind=", (int)item.kind, " doesn't belong to any ItemType");

    return result;
}

auto ItemActivator::info(int itemIdx, GameState& gameState) -> bn::vector<bn::string_view, 2>
{
    const auto& items = gameState.getItems();
    BN_ASSERT(0 <= itemIdx && itemIdx < items.size(), "itemIdx=", itemIdx, " OOB (size=", items.size(), ")");
    const auto& item = ItemInfo::get(items[itemIdx]);

    return item.getDescriptions();
}

auto ItemActivator::drop(int itemIdx, GameState& gameState) -> bn::string<consts::DIALOG_MAX_CHARS>
{
    auto& items = gameState.getItems();
    BN_ASSERT(0 <= itemIdx && itemIdx < items.size(), "itemIdx=", itemIdx, " OOB (size=", items.size(), ")");
    const auto& item = ItemInfo::get(items[itemIdx]);

    if (item.kind == ItemKind::ANNOYING_DOG)
    {
        // TODO: Trigger Annoying Dog event.

        items.erase(items.begin() + itemIdx);
        return getTextEn(TextData::item_drop_27);
    }
    else if (item.kind == ItemKind::BAD_MEMORY)
    {
        return getTextEn(TextData::item_drop_54);
    }
    else if (item.kind == ItemKind::UNDYNES_LETTER)
    {
        // TODO
        // if (Undyne is in the current room)
        //     return getTextEn(TextData::item_drop_56_undyne);

        items.erase(items.begin() + itemIdx);
        return getTextEn(TextData::item_drop_56);
    }
    else if (item.kind == ItemKind::UNDYNES_LETTER_EX)
    {
        return getTextEn(TextData::item_drop_57);
    }

    items.erase(items.begin() + itemIdx);
    return bn::format<consts::DIALOG_MAX_CHARS>(R"(* The {} was&  thrown away.)", item.getName());
}

} // namespace ut::game
