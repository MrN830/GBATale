#include "core/TextGens.hpp"

#include "asset/font_crypt.hpp"
#include "asset/font_main.hpp"

namespace ut::core
{

TextGens::TextGens()
    : _textGens{bn::sprite_text_generator(asset::font_main), bn::sprite_text_generator(asset::font_crypt)}
{
}

auto TextGens::get(asset::FontKind font) -> bn::sprite_text_generator&
{
    BN_ASSERT(0 <= (int)font && (int)font < (int)asset::FontKind::TOTAL_COUNT);

    return _textGens[(int)font];
}

} // namespace ut::core
