#pragma once

#include <cstdint>

#include "util/enum_as_flags.hpp"

namespace ut::game::cpnt::inter
{

enum class InteractionTriggers : uint8_t
{
    NONE = 0,

    PRESS_A = (1 << 0),
    COLLIDE_FRISK = (1 << 1),
    COLLIDE_OTHERS = (1 << 2),

    COLLIDE = COLLIDE_FRISK | COLLIDE_OTHERS
};

ENUM_AS_FLAGS(InteractionTriggers);

} // namespace ut::game::cpnt::inter
