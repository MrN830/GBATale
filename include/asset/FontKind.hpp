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
    CRYPT,
    TINY,

    TOTAL_COUNT
};

auto getFont(FontKind) -> const bn::sprite_font&;

} // namespace ut::asset
