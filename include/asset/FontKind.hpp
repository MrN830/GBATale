#pragma once

namespace bn
{
class sprite_font;
}

namespace ut::asset
{

enum class FontKind
{
    MAIN,
    MAIN_FIXED_WIDTH,
    CRYPT,
    TINY,

    TOTAL_COUNT
};

auto getFont(FontKind) -> const bn::sprite_font&;

} // namespace ut::asset
