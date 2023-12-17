#pragma once

#include <cstdint>

namespace ut::game
{

/**
 * @brief Undertale's persistent save data (undertale.ini)
 *
 * Variable names follow the original as-is, including not-normalized casing.
 */
struct PersistData
{
    struct Toriel
    {
        enum class Bscotch : uint8_t
        {
            INIT = 0,
            BUTTERSCOTCH = 1,
            CINNAMON = 2,
        };
        Bscotch Bscotch = Bscotch::INIT;

        uint32_t TS = 0; // toriel spared count
        uint32_t TK = 0; // toriel killed count
    };
    Toriel Toriel;

    struct Flowey
    {
        bool Alter : 1 = false;
        bool K : 1 = false;
        bool SPECIALK : 1 = false;
        bool alter2 : 1 = false;
        bool FloweyExplain1 : 1 = false;
        bool truename : 1 = false; // Flowey calls the protag "Chara"
    };
    Flowey Flowey;
};

} // namespace ut::game
