#include "game/bt/MonsterInfo.hpp"

#include <bn_assert.h>

#include "game/bt/MonsterKind.hpp"

#include "gen/TextData.hpp"

namespace ut::game::bt
{

auto MonsterInfo::getName() const -> const bn::string_view&
{
    // TODO: get text data considering language
    return asset::gen::getTextEn(nameData);
}

namespace
{

using Text = asset::gen::TextData;

constexpr MonsterInfo MOBS[(int)MonsterKind::TOTAL_COUNT] = {
    {
        MonsterKind::NONE,
        0,
        0,
        0,
        0,
        0,
        Text::NONE,
    },
    {
        MonsterKind::TEST_FROGGIT,
        23,
        4,
        1,
        2,
        2,
        Text::monstername_1,
    },
    {
        MonsterKind::DUMMY,
        15,
        0,
        -5,
        0,
        0,
        Text::monstername_2,
    },
    {
        MonsterKind::FAKE_FROGGIT,
        20,
        4,
        1,
        10,
        20,
        Text::monstername_3,
    },
    {
        MonsterKind::FROGGIT,
        30,
        4,
        4,
        3,
        2,
        Text::monstername_4,
    },
    {
        MonsterKind::WHIMSUN,
        10,
        4,
        0,
        2,
        2,
        Text::monstername_5,
    },
    {
        MonsterKind::MOLDSMAL_1,
        50,
        4,
        0,
        3,
        3,
        Text::monstername_6,
    },
    {
        MonsterKind::MIGOSP,
        40,
        5,
        4,
        5,
        4,
        Text::monstername_7,
    },
    {
        MonsterKind::VEGETOID,
        72,
        5,
        0,
        6,
        1,
        Text::monstername_8,
    },
    {
        MonsterKind::LOOX,
        50,
        5,
        4,
        7,
        5,
        Text::monstername_9,
    },
    {
        MonsterKind::TORIEL,
        440,
        6,
        1,
        0,
        0,
        Text::monstername_10,
    },
    {
        MonsterKind::NAPSTABLOOK,
        88,
        5,
        4,
        -1,
        0,
        Text::monstername_11,
    },
    {
        MonsterKind::MOLDSMAL_2,
        50,
        7,
        0,
        3,
        3,
        Text::monstername_12,
    },
};

} // namespace

auto MonsterInfo::get(MonsterKind kind) -> const MonsterInfo&
{
    BN_ASSERT((int)kind < (int)MonsterKind::TOTAL_COUNT);

    return MOBS[(int)kind];
}

} // namespace ut::game::bt
