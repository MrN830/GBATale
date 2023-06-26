#pragma once

namespace ut::game::menu
{

enum class MenuStateType
{
    CLOSE_MENU = -2,
    NONE = -1,

    MAIN = 0,
    DIALOG,
    ITEM,
    STAT,
    CELL,
};

} // namespace ut::game::menu
