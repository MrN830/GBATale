#pragma once

#include <cstdint>

namespace ut::core
{

enum class ChoiceMsgKind : uint16_t
{
    NONE = 0,

    YELLOW_NAME_HELPFUL,
    YELLOW_NAME_BAD,

    KEEP_YELLOW_NAMES,
    NO_MORE_YELLOW_NAMES,

    NO_NAME_COLOR_GREAT,
    BRING_NAME_COLOR_BACK,

    TORIEL_DIARY_YES,
    TORIEL_DIARY_NO,
};

}; // namespace ut::core
