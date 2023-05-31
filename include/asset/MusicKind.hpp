#pragma once

namespace bn
{
class music_item;
}

namespace ut::asset
{

enum class MusicKind
{
    NONE = -1,

    TITLE_MENU_1 = 0,

    FALLEN_DOWN,

    TOTAL_COUNT
};

auto getMusic(MusicKind) -> const bn::music_item*;

} // namespace ut::asset
