#pragma once

#include <bn_algorithm.h>

#include "game/menu/MainMenu.hpp"
#include "game/menu/StatMenu.hpp"

namespace ut::game::menu
{

inline constexpr int MENU_ALIGN_SIZE = bn::max({alignof(MainMenu), alignof(StatMenu)});
inline constexpr int MENU_MAX_SIZE = bn::max({sizeof(MainMenu), sizeof(StatMenu)});

} // namespace ut::game::menu
