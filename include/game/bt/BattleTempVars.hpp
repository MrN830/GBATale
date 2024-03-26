#pragma once

#include <bn_array.h>
#include <bn_string_view.h>
#include <bn_vector.h>

#include "bt_consts.hpp"
#include "consts.hpp"

namespace ut::game::bt
{

/**
 * @brief Battle temp variables that's shared between battle states.
 */
struct BattleTempVars
{
    int submenuMainCursorIdx = 0;
    int submenuMobSelectIdx = 0;

    // [0]: main, monster[0]
    // [1..2]: monster[1..2]
    bn::array<bn::vector<bn::string_view, ut::consts::DIALOG_MAX_LINES>, bt::consts::MAX_MOBS> dialogs;
};

} // namespace ut::game::bt
