#pragma once

namespace bn
{
class music_item;
}

namespace ut::asset
{

enum class MusicKind
{
    TITLE_MENU_1,

    FALLEN_DOWN,

    TOTAL_COUNT
};

auto getMusic(MusicKind) -> const bn::music_item&;

} // namespace ut::asset
