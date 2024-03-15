#include "core/BgBox.hpp"

#include <bn_memory.h>
#include <bn_regular_bg_item.h>

#ifdef UT_BG_BOX_DEBUG
#include "bn_regular_bg_tiles_items_debug_numbers.h"
#endif

namespace ut::core
{

BgBox::BgBox(const bn::top_left_fixed_rect& boxRect, int borderThickness, bn::optional<bn::color> borderColor,
             bn::optional<bn::color> fillColor
#ifdef UT_BG_BOX_DEBUG
             ,
             bool debug
#endif
             )
    :
#ifdef UT_BG_BOX_DEBUG
      _debug(debug),
#endif
      _borderThickness(borderThickness), _borderColorIdx([](int thickness, bool has_border_color) {
          if (thickness <= 0)
              return 2;
          if (!has_border_color)
              return 0;
          return 1;
      }(borderThickness, borderColor.has_value())),
      _fillColorIdx([](bool has_fill_color) { return has_fill_color ? 2 : 0; }(fillColor.has_value())), _cells{},
      _tiles{}, _colors{}, _usedTilePos{}, _mapItem(_cells[0], MAP_SIZE),
      _palette(bn::bg_palette_item(_colors, bn::bpp_mode::BPP_4).create_new_palette()),
      _tileset(bn::regular_bg_tiles_item(_tiles, bn::bpp_mode::BPP_4).create_tiles()),
      _map(_mapItem.create_new_map(_tileset, _palette)), _bg(bn::regular_bg_ptr::create(0, 0, _map))
{
    BN_ASSERT(0 <= borderThickness && borderThickness <= 8, "Invalid thickness: ", borderThickness);

    BN_ASSERT(!(borderThickness == 0 && !fillColor), "`BgBox` is always invisible");
    BN_ASSERT(!(borderThickness > 0 && !borderColor && !fillColor), "`BgBox` is always invisible");

    if (borderThickness > 0 && borderColor.has_value())
    {
        BN_ASSERT(_borderColorIdx == 1);
        _colors[_borderColorIdx] = *borderColor;
    }

    if (fillColor.has_value())
    {
        BN_ASSERT(_fillColorIdx == 2);
        _colors[_fillColorIdx] = *fillColor;
    }

    _palette.set_colors(bn::bg_palette_item(_colors, bn::bpp_mode::BPP_4));

#ifdef UT_BG_BOX_DEBUG
    if (debug)
    {
        for (int i = 0; i < UNIQUE_TILE_COUNT; ++i)
            _tiles[i] = bn::regular_bg_tiles_items::debug_numbers.tiles_ref()[i];

        _tileset.reload_tiles_ref();
    }
    else
#endif
    {
        const uint32_t midColor = (_fillColorIdx << 0u) | (_fillColorIdx << 4u) | (_fillColorIdx << 8u) |
                                  (_fillColorIdx << 12u) | (_fillColorIdx << 16u) | (_fillColorIdx << 20u) |
                                  (_fillColorIdx << 24u) | (_fillColorIdx << 28u);

        bn::memory::set_words(midColor, sizeof(_tiles[TileIdx::MID_INNER].data) / 4, _tiles[TileIdx::MID_INNER].data);
    }

    setRect(boxRect);
}

auto BgBox::getRect() const -> const bn::top_left_fixed_rect&
{
    return _rawRect;
}

auto BgBox::getPosition() const -> const bn::fixed_point&
{
    return _rawRect.position();
}

auto BgBox::getWidth() const -> bn::fixed
{
    return _rawRect.width();
}

auto BgBox::getHeight() const -> bn::fixed
{
    return _rawRect.height();
}

void BgBox::setRect(const bn::top_left_fixed_rect& boxRect)
{
    if (_rawRect == boxRect)
        return;

    BN_ASSERT(boxRect.width() >= 2 * _borderThickness, "width is too thin: ", boxRect.width(), " - ",
              2 * _borderThickness);
    BN_ASSERT(boxRect.height() >= 2 * _borderThickness, "height is too thin: ", boxRect.height(), " - ",
              2 * _borderThickness);

    _rawRect = boxRect;

    redraw();
}

void BgBox::setPosition(const bn::fixed_point& position)
{
    auto newRect = _rawRect;
    newRect.set_position(position);
    setRect(newRect);
}

void BgBox::setWidth(bn::fixed width)
{
    auto newRect = _rawRect;
    newRect.set_width(width);
    setRect(newRect);
}

void BgBox::setHeight(bn::fixed height)
{
    auto newRect = _rawRect;
    newRect.set_height(height);
    setRect(newRect);
}

int BgBox::getBorderThickness() const
{
    return _borderThickness;
}

auto BgBox::getCanvas() -> bn::regular_bg_ptr&
{
    return _bg;
}

auto BgBox::getCanvas() const -> const bn::regular_bg_ptr&
{
    return _bg;
}

auto BgBox::getClampedRect() const -> bn::top_left_fixed_rect
{
    auto clamped = _rawRect;

    if (clamped.left() < -MAP_LEN.width() / 2)
    {
        clamped.set_width(bn::max(bn::fixed(0), clamped.width() - (-MAP_LEN.width() / 2 - clamped.x())));
        clamped.set_x(-MAP_LEN.width() / 2);
    }
    if (clamped.right() > MAP_LEN.width() / 2)
    {
        clamped.set_width(bn::max(bn::fixed(0), clamped.width() - (clamped.right() - MAP_LEN.width() / 2)));
    }
    if (clamped.top() < -MAP_LEN.height() / 2)
    {
        clamped.set_height(bn::max(bn::fixed(0), clamped.height() - (-MAP_LEN.height() / 2 - clamped.y())));
        clamped.set_y(-MAP_LEN.height() / 2);
    }
    if (clamped.bottom() > MAP_LEN.height() / 2)
    {
        clamped.set_height(bn::max(bn::fixed(0), clamped.height() - (clamped.bottom() - MAP_LEN.height() / 2)));
    }

    return clamped;
}

auto BgBox::convertToPositiveRect(const bn::top_left_fixed_rect& rawRect) -> bn::top_left_fixed_rect
{
    return bn::top_left_fixed_rect{
        rawRect.position().x() + MAP_LEN.width() / 2,
        rawRect.position().y() + MAP_LEN.height() / 2,
        rawRect.width(),
        rawRect.height(),
    };
}

auto BgBox::convertToPositiveIntRect(const bn::top_left_fixed_rect& rawRect) -> bn::top_left_rect
{
    return bn::top_left_rect{
        (rawRect.position().x() + MAP_LEN.width() / 2).floor_integer(),
        (rawRect.position().y() + MAP_LEN.height() / 2).floor_integer(),
        rawRect.width().round_integer(),
        rawRect.height().round_integer(),
    };
}

} // namespace ut::core
