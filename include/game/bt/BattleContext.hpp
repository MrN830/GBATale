#pragma once

#include <cstdint>

namespace ut::game::bt
{

/**
 * @brief Battle context that's set outside of battle.
 */
struct BattleContext
{
    uint8_t battleGroupId = 0;
};

} // namespace ut::game::bt
