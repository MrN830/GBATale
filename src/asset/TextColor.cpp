#include "asset/TextColor.hpp"

#include "bn_sprite_palette_items_pal_ftmain_azure.h"
#include "bn_sprite_palette_items_pal_ftmain_black.h"
#include "bn_sprite_palette_items_pal_ftmain_blue.h"
#include "bn_sprite_palette_items_pal_ftmain_gray.h"
#include "bn_sprite_palette_items_pal_ftmain_green.h"
#include "bn_sprite_palette_items_pal_ftmain_lightgray.h"
#include "bn_sprite_palette_items_pal_ftmain_magenta.h"
#include "bn_sprite_palette_items_pal_ftmain_orange.h"
#include "bn_sprite_palette_items_pal_ftmain_pink.h"
#include "bn_sprite_palette_items_pal_ftmain_red.h"
#include "bn_sprite_palette_items_pal_ftmain_white.h"
#include "bn_sprite_palette_items_pal_ftmain_yellow.h"

namespace ut::asset
{

static constexpr const bn::sprite_palette_item* COLORS[(int)TextColorKind::TOTAL_COUNT] = {
    &bn::sprite_palette_items::pal_ftmain_red,     &bn::sprite_palette_items::pal_ftmain_green,
    &bn::sprite_palette_items::pal_ftmain_white,   &bn::sprite_palette_items::pal_ftmain_lightgray,
    &bn::sprite_palette_items::pal_ftmain_gray,    &bn::sprite_palette_items::pal_ftmain_yellow,
    &bn::sprite_palette_items::pal_ftmain_black,   &bn::sprite_palette_items::pal_ftmain_blue,
    &bn::sprite_palette_items::pal_ftmain_orange,  &bn::sprite_palette_items::pal_ftmain_azure,
    &bn::sprite_palette_items::pal_ftmain_magenta, &bn::sprite_palette_items::pal_ftmain_pink,
};

auto getTextColor(TextColorKind kind) -> const bn::sprite_palette_item&
{
    BN_ASSERT(0 <= (int)kind && (int)kind < (int)TextColorKind::TOTAL_COUNT);

    return *COLORS[(int)kind];
}

} // namespace ut::asset
