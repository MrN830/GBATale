#pragma once

namespace ut::game::bt::state
{

enum class BattleStateType
{
    END_BATTLE = -2,
    NONE = -1,

    BATTLE_MENU = 0,
    BATTLE_ATTACK,
    BATTLE_PREPARE_DODGE,
    BATTLE_DODGE,
    BATTLE_END_DODGE,
};

} // namespace ut::game::bt::state
