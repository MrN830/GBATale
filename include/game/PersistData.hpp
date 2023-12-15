#pragma once

#include <cstdint>

namespace ut::game
{

/**
 * @brief Undertale's persistent save data (undertale.ini)
 *
 * Variable names follow the original.
 */
struct PersistData
{
    enum class TorielBscotch : uint8_t
    {
        INIT = 0,
        BUTTERSCOTCH = 1,
        CINNAMON = 2,
    };

    TorielBscotch toriel_bscotch = TorielBscotch::INIT;
};

} // namespace ut::game
