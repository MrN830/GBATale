#pragma once

namespace bn
{
class sprite_palette_item;
}

namespace ut::asset
{

enum class TextColorKind
{
    RED,
    GREEN,
    WHITE,
    LIGHT_GRAY,
    GRAY,
    YELLOW,
    BLACK,
    BLUE,
    ORANGE,
    AZURE,
    MAGENTA,
    PINK,

    TOTAL_COUNT
};

auto getTextColor(TextColorKind) -> const bn::sprite_palette_item&;

} // namespace ut::asset
