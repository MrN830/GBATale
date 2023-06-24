#pragma once

#include <cstdint>

namespace ut::game
{

enum class ItemKind : uint8_t;

enum class ItemType
{
    FOOD,
    WEAPON,
    ARMOR,
    SPECIAL
};

struct ItemInfo
{
public:
    static auto get(ItemKind) -> const ItemInfo&;

public:
    bn::string_view name, shortName, seriousName;
    bn::string_view description;
    int hpRegen;
    int battleStat; // atk or def
    int price;      // `-1` if not tradable
};

enum class ItemKind : uint8_t
{
    NONE = 0,

    MONSTER_CANDY = 1,
    CROQUET_ROLL,
    STICK,
    BANDAGE,
    ROCK_CANDY,
    PUMPKIN_RINGS,
    SPIDER_DONUT,
    STOIC_ONION,
    GHOST_FRUIT,
    SPIDER_CIDER,
    BUTTERSCOTCH_PIE,
    FADED_RIBBON,
    TOY_KNIFE,
    TOUGH_GLOVE,
    MANLY_BANDANNA,
    SNOWMAN_PIECE,
    NICE_CREAM,
    PUPPYDOUGH_ICECREAM,
    BISICLE,
    UNISICLE,
    CINNAMON_BUNNY,
    TEMMIE_FLAKES,
    ABANDONED_QUICHE,
    OLD_TUTU,
    BALLET_SHOES,
    PUNCH_CARD,
    ANNOYING_DOG,
    DOG_SALAD,
    DOG_RESIDUE_1,
    DOG_RESIDUE_2,
    DOG_RESIDUE_3,
    DOG_RESIDUE_4,
    DOG_RESIDUE_5,
    DOG_RESIDUE_6,
    ASTRONAUT_FOOD,
    INSTANT_NOODLES,
    CRAB_APPLE,
    HOT_DOG,
    HOT_CAT,
    GLAMBURGER,
    SEA_TEA,
    STARFAIT,
    LEGENDARY_HERO,
    CLOUDY_GLASSES,
    TORN_NOTEBOOK,
    STAINED_APRON,
    BURNT_PAN,
    COWBOY_HAT,
    EMPTY_GUN,
    HEART_LOCKET,
    WORN_DAGGER,
    REAL_KNIFE,
    THE_LOCKET,
    BAD_MEMORY,
    DREAM,
    UNDYNES_LETTER,
    UNDYNES_LETTER_EX,
    POPATO_CHISPS,
    JUNK_FOOD,
    MYSTERY_KEY,
    FACE_STEAK,
    HUSH_PUPPY,
    SNAIL_PIE,
    TEMY_ARMOR,

    TOTAL_COUNT
};

} // namespace ut::game
