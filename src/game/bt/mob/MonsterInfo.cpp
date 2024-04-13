#include "game/bt/mob/MonsterInfo.hpp"

#include <bn_assert.h>

#include "asset/SfxKind.hpp"
#include "game/bt/mob/MonsterKind.hpp"

#include "gen/TextData.hpp"

namespace ut::game::bt::mob
{

auto MonsterInfo::getName() const -> const bn::string_view&
{
    // TODO: get text data considering language
    return asset::gen::getTextEn(nameData);
}

namespace
{

using Text = asset::gen::TextData;
using Sfx = asset::SfxKind;

constexpr MonsterInfo MOBS[(int)MonsterKind::TOTAL_COUNT] = {
    {
        MonsterKind::NONE,
        0,
        0,
        0,
        0,
        0,
        Text::NONE,
        Sfx::NONE,
    },
    {
        MonsterKind::TEST_FROGGIT,
        23,
        4,
        1,
        2,
        2,
        Text::monstername_1,
        Sfx::MOB_CRY_FROGGIT,
    },
    {
        MonsterKind::DUMMY,
        15,
        0,
        -5,
        0,
        0,
        Text::monstername_2,
        Sfx::NONE,
    },
    {
        MonsterKind::FAKE_FROGGIT,
        20,
        4,
        1,
        10,
        20,
        Text::monstername_3,
        Sfx::MOB_CRY_FROGGIT,
    },
    {
        MonsterKind::FROGGIT,
        30,
        4,
        4,
        3,
        2,
        Text::monstername_4,
        Sfx::MOB_CRY_FROGGIT,
    },
    {
        MonsterKind::WHIMSUN,
        10,
        4,
        0,
        2,
        2,
        Text::monstername_5,
        Sfx::NONE,
    },
    {
        MonsterKind::MOLDSMAL_1,
        50,
        4,
        0,
        3,
        3,
        Text::monstername_6,
        Sfx::NONE,
    },
    {
        MonsterKind::MIGOSP,
        40,
        5,
        4,
        5,
        4,
        Text::monstername_7,
        Sfx::NONE,
    },
    {
        MonsterKind::VEGETOID,
        72,
        5,
        0,
        6,
        1,
        Text::monstername_8,
        Sfx::NONE,
    },
    {
        MonsterKind::LOOX,
        50,
        5,
        4,
        7,
        5,
        Text::monstername_9,
        Sfx::NONE,
    },
    {
        MonsterKind::TORIEL,
        440,
        6,
        1,
        0,
        0,
        Text::monstername_10,
        Sfx::NONE,
    },
    {
        MonsterKind::NAPSTABLOOK,
        88,
        5,
        4,
        -1,
        0,
        Text::monstername_11,
        Sfx::NONE,
    },
    {
        MonsterKind::MOLDSMAL_2,
        50,
        7,
        0,
        3,
        3,
        Text::monstername_12,
        Sfx::NONE,
    },
};

} // namespace

auto MonsterInfo::get(MonsterKind kind) -> const MonsterInfo&
{
    BN_ASSERT((int)kind < (int)MonsterKind::TOTAL_COUNT);

    return MOBS[(int)kind];
}

} // namespace ut::game::bt::mob
