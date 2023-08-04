#include "core/TextGens.hpp"

#include "asset/font_crypt.hpp"
#include "asset/font_main.hpp"
#include "asset/font_tiny.hpp"

namespace ut::core
{

TextGens::TextGens()
    : _textGens{bn::sprite_text_generator(asset::font_main), bn::sprite_text_generator(asset::font_main_fixed_width),
                bn::sprite_text_generator(asset::font_crypt), bn::sprite_text_generator(asset::font_tiny)}
{
}

auto TextGens::get(asset::FontKind font) -> bn::sprite_text_generator&
{
    BN_ASSERT(0 <= (int)font && (int)font < (int)asset::FontKind::TOTAL_COUNT);

    return _textGens[(int)font];
}

} // namespace ut::core
