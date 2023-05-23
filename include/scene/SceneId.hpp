#pragma once

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

#ifdef UT_TEST
    SAVE_TEST,
#endif

    TOTAL_COUNT
};

} // namespace ut::scene
