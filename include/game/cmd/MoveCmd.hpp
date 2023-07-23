#pragma once

#include <bn_fixed_point.h>

#include "core/Directions.hpp"

namespace ut::game::cmd
{

struct MoveCmd
{
    core::Directions directions = core::Directions::NONE;
    bn::fixed_point movePos;
    bool checkCollision : 1 = true;
};

} // namespace ut::game::cmd
