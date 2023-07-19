#pragma once

#include <cstdint>

namespace ut::game
{

enum class InteractState : int8_t
{
    FREE,
    INTERACT,
    CUTSCENE,
    ROOM_CHANGE,
    FALLING,
    MENU,
};

} // namespace ut::game
