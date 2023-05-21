#include "asset/FontKind.hpp"

#include "asset/font_crypt.hpp"
#include "asset/font_main.hpp"

namespace ut::asset
{

static constexpr const bn::sprite_font* FONTS[(int)FontKind::TOTAL_COUNT] = {
    &font_main,
    &font_crypt,
};

auto getFont(FontKind kind) -> const bn::sprite_font&
{
    BN_ASSERT(0 <= (int)kind && (int)kind < (int)FontKind::TOTAL_COUNT);

    return *FONTS[(int)kind];
}

} // namespace ut::asset
