#pragma once

#include <bn_fixed_point.h>

#include "core/Directions.hpp"

namespace ut::game::cmd
{

struct InputCmd
{
    core::Directions directions = core::Directions::NONE;
    bn::fixed_point movePos;
    bool checkCollision : 1 = true;

    bool confirm : 1 = false;
    bool cancel : 1 = false;
    bool menu : 1 = false;
};

} // namespace ut::game::cmd
