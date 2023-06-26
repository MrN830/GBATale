#pragma once

#include <bn_algorithm.h>

#include "game/menu/CellMenu.hpp"
#include "game/menu/DialogMenu.hpp"
#include "game/menu/ItemMenu.hpp"
#include "game/menu/MainMenu.hpp"
#include "game/menu/StatMenu.hpp"

namespace ut::game::menu
{

inline constexpr int MENU_ALIGN_SIZE =
    bn::max({alignof(MainMenu), alignof(DialogMenu), alignof(ItemMenu), alignof(StatMenu), alignof(CellMenu)});
inline constexpr int MENU_MAX_SIZE =
    bn::max({sizeof(MainMenu), sizeof(DialogMenu), sizeof(ItemMenu), sizeof(StatMenu), sizeof(CellMenu)});

} // namespace ut::game::menu
