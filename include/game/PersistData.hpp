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

        bool AF : 1 = false;
        bool AK : 1 = false;
        bool NK : 1 = false;
        bool IK : 1 = false;

        enum class Met1 : uint8_t
        {
            NEVER_MET = 0,
            MET_ONCE = 1,
            MET_MORE = 2,
        };
        Met1 Met1 = Met1::NEVER_MET;
    };
    Flowey Flowey;
};

} // namespace ut::game
