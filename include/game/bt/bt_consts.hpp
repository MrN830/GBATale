#pragma once

#include <bn_display.h>
#include <bn_top_left_fixed_rect.h>

namespace ut::game::bt
{

inline constexpr bn::top_left_fixed_rect BG_BOX_INIT_RECT = {
    2 - bn::display::width() / 2,
    102 - bn::display::height() / 2,
    236,
    56,
};

}
