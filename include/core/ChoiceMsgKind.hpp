#pragma once

#include <cstdint>

namespace ut::core
{

enum class ChoiceMsgKind : uint16_t
{
    NONE = 0,

    CLOSE_IMMEDIATELY,

    TAKE_CANDY_YES,
    TAKE_CANDY_NO,

    YELLOW_NAME_HELPFUL,
    YELLOW_NAME_BAD,

    KEEP_YELLOW_NAMES,
    NO_MORE_YELLOW_NAMES,

    NO_NAME_COLOR_GREAT,
    BRING_NAME_COLOR_BACK,

    TORIEL_DIARY_YES,

    PRESS_COLOR_SWITCH_OPEN,
    PRESS_COLOR_SWITCH_NOTHING_HAPPENED,
    PRESS_COLOR_SWITCH_TOO_MANY_TIMES,
};

}; // namespace ut::core
