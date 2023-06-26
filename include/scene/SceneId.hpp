#pragma once

#include "config.hpp"

namespace ut::scene
{

enum class SceneId
{
    NONE = -1,

    INTRO_STORY = 0,
    INTRO_LOGO,
    TITLE,
    NEW_GAME_TITLE,
    INPUT_NAME,
    CONFIRM_NAME,

    GAME,
    INGAME_MENU,

#if UT_TEST_SCENE
    SAVE_TEST,
    WORLD_BG_TEST,
#endif

    TOTAL_COUNT
};

} // namespace ut::scene
