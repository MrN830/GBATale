#pragma once

#include <bn_sprite_text_generator.h>

namespace ut::core
{

class TextGens
{
public:
    enum class Font
    {
        MAIN,
        CRYPT,

        TOTAL_COUNT
    };

public:
    TextGens();

    auto get(Font) -> bn::sprite_text_generator&;

private:
    bn::sprite_text_generator _textGens[(int)Font::TOTAL_COUNT];
};

} // namespace ut::core
