#pragma once

#include <bn_optional.h>
#include <bn_regular_bg_ptr.h>

#include "game/bt/BattleBgKind.hpp"

namespace ut::game::bt
{

class BattleBg final
{
public:
    BattleBg(BattleBgKind);

private:
    bn::optional<bn::regular_bg_ptr> _bg;
};

} // namespace ut::game::bt
