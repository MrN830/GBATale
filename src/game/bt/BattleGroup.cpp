#include "game/bt/BattleGroup.hpp"

#include <bn_assert.h>

#include "asset/BgmKind.hpp"
#include "game/bt/MonsterKind.hpp"

#include "gen/TextData.hpp"

namespace ut::game::bt
{

namespace
{

constexpr int MAX_GROUPS = 23;

using Mob = MonsterKind;
using Text = asset::gen::TextData;
using Bgm = asset::BgmKind;

constexpr BattleGroup GROUPS[MAX_GROUPS] = {
    {
        0,
        {Mob::NONE, Mob::NONE, Mob::NONE},
        Text::NONE,
        Bgm::NONE,
    },
    {
        1,
        {Mob::TEST_FROGGIT, Mob::TEST_FROGGIT, Mob::TEST_FROGGIT},
        Text::SCR_TEXT_7084,
        Bgm::ENEMY_APPROACHING,
    },
    {
        2,
        {Mob::DUMMY, Mob::NONE, Mob::NONE},
        Text::SCR_TEXT_7079,
        Bgm::ANTICIPATION,
    },
    {
        3,
        {Mob::FAKE_FROGGIT, Mob::NONE, Mob::NONE},
        Text::SCR_TEXT_7084,
        Bgm::ANTICIPATION,
    },
    {
        4,
        {Mob::FROGGIT, Mob::NONE, Mob::NONE},
        Text::SCR_TEXT_7089,
        Bgm::ENEMY_APPROACHING,
    },
    {
        5,
        {Mob::WHIMSUN, Mob::NONE, Mob::NONE},
        Text::SCR_TEXT_7094,
        Bgm::ENEMY_APPROACHING,
    },
    {
        6,
        {Mob::FROGGIT, Mob::WHIMSUN, Mob::NONE},
        Text::SCR_TEXT_7099,
        Bgm::ENEMY_APPROACHING,
    },
    {
        7,
        {Mob::MOLDSMAL_1, Mob::NONE, Mob::NONE},
        Text::SCR_TEXT_7104,
        Bgm::ENEMY_APPROACHING,
    },
    {
        8,
        {Mob::MOLDSMAL_1, Mob::MOLDSMAL_1, Mob::MOLDSMAL_1},
        Text::scr_battlegroup_116,
        Bgm::ENEMY_APPROACHING,
    },
    {
        9,
        {Mob::FROGGIT, Mob::FROGGIT, Mob::NONE},
        Text::scr_battlegroup_133,
        Bgm::ENEMY_APPROACHING,
    },
    {
        10,
        {Mob::MOLDSMAL_1, Mob::MOLDSMAL_1, Mob::NONE},
        Text::scr_battlegroup_148,
        Bgm::ENEMY_APPROACHING,
    },
    {
        11,
        {Mob::MOLDSMAL_1, Mob::MIGOSP, Mob::NONE},
        Text::scr_battlegroup_163,
        Bgm::ENEMY_APPROACHING,
    },
    {
        12,
        {Mob::MIGOSP, Mob::VEGETOID, Mob::NONE},
        Text::scr_battlegroup_178,
        Bgm::ENEMY_APPROACHING,
    },
    {
        13,
        {Mob::LOOX, Mob::NONE, Mob::NONE},
        Text::scr_battlegroup_193,
        Bgm::ENEMY_APPROACHING,
    },
    {
        14,
        {Mob::LOOX, Mob::VEGETOID, Mob::NONE},
        Text::scr_battlegroup_207,
        Bgm::ENEMY_APPROACHING,
    },
    {
        15,
        {Mob::LOOX, Mob::VEGETOID, Mob::MIGOSP},
        Text::scr_battlegroup_222,
        Bgm::ENEMY_APPROACHING,
    },
    {
        16,
        {Mob::VEGETOID, Mob::VEGETOID, Mob::NONE},
        Text::scr_battlegroup_238,
        Bgm::ENEMY_APPROACHING,
    },
    {
        17,
        {Mob::LOOX, Mob::LOOX, Mob::NONE},
        Text::scr_battlegroup_253,
        Bgm::ENEMY_APPROACHING,
    },
    {
        18,
        {Mob::VEGETOID, Mob::NONE, Mob::NONE},
        Text::scr_battlegroup_268,
        Bgm::ENEMY_APPROACHING,
    },
    {
        19, // * But nobody came.
        {Mob::NONE, Mob::NONE, Mob::NONE},
        Text::scr_battlegroup_282,
        Bgm::NONE,
    },
    {
        20,
        {Mob::NAPSTABLOOK, Mob::NONE, Mob::NONE},
        Text::scr_battlegroup_297, // TODO: 299
        Bgm::GHOST_FIGHT,
    },
    {
        21,
        {Mob::LOOX, Mob::NONE, Mob::NONE},
        Text::scr_battlegroup_193,
        Bgm::ENEMY_APPROACHING,
    },
    {
        22,
        {Mob::TORIEL, Mob::NONE, Mob::NONE},
        Text::scr_battlegroup_327,
        Bgm::HEARTACHE,
    },
};

} // namespace

auto BattleGroup::get(uint8_t id) -> const BattleGroup&
{
    BN_ASSERT(id < MAX_GROUPS);

    return GROUPS[id];
}

} // namespace ut::game::bt
