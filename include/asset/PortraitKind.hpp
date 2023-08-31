#pragma once

#include <cstdint>

namespace ut::asset
{

enum class PortraitFaceKind : uint8_t
{
    NONE = 0,

    TORIEL = 1,
    FLOWEY = 2,
    SANS = 3,
    PAPYRUS = 4,
    UNDYNE = 5,
    ALPHYS = 6,
    ASGORE = 7,
    METTATON = 8,
    ASRIEL = 9,

    TOTAL_COUNT
};

} // namespace ut::asset
