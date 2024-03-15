#pragma once

#include <bn_display.h>
#include <bn_top_left_fixed_rect.h>

namespace ut::game::bt::consts
{

inline constexpr int BG_BOX_PRIORITY = 2;
inline constexpr int BG_BACKDROP_PRIORITY = 3;

inline constexpr bn::top_left_fixed_rect BG_BOX_INIT_RECT = {
    2 - bn::display::width() / 2,
    102 - bn::display::height() / 2,
    236,
    56,
};

} // namespace ut::game::bt::consts
