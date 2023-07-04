#pragma once

#include <bn_fixed_point.h>

namespace ut::game::cmd
{

struct InputCmd
{
    bn::fixed_point movePos;
    bool confirm : 1 = false;
    bool cancel : 1 = false;
    bool menu : 1 = false;
};

} // namespace ut::game::cmd
