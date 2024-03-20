#pragma once

#include <cstdint>

namespace ut::game::bt::mob
{

enum class MonsterKind : uint8_t
{
    NONE = 0,

    TEST_FROGGIT = 1,

    DUMMY = 2,
    FAKE_FROGGIT = 3,
    FROGGIT = 4,
    WHIMSUN = 5,
    MOLDSMAL_1 = 6,
    MIGOSP = 7,
    VEGETOID = 8,
    LOOX = 9,
    TORIEL = 10,
    NAPSTABLOOK = 11,
    MOLDSMAL_2 = 12,

    TOTAL_COUNT,
};

} // namespace ut::game::bt
