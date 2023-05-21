#pragma once

#include <bn_sprite_text_generator.h>

#include "asset/FontKind.hpp"

namespace ut::core
{

class TextGens
{
public:
    TextGens();

    auto get(asset::FontKind) -> bn::sprite_text_generator&;

private:
    bn::sprite_text_generator _textGens[(int)asset::FontKind::TOTAL_COUNT];
};

} // namespace ut::core
