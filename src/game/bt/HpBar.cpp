#include "game/bt/HpBar.hpp"

#include <bn_memory.h>
#include <bn_sprite_palette_item.h>
#include <bn_sprite_tiles_item.h>

namespace ut::game::bt
{

namespace
{

enum ColorIdx : uint32_t
{
    TRANSPARENT = 0,

    HEALTH = 1,
    DAMAGE = 2,

    BORDER = 3,
};

inline auto getBorderDiff(bool hasBorder) -> bn::fixed_point
{
    return (hasBorder) ? bn::fixed_point(-1, -1) : bn::fixed_point(0, 0);
}

inline ColorIdx getPlotInner(int dotX, bool hasBorder, int healthWidth, int totalWidth)
{
    if (hasBorder)
    {
        if (dotX == 0 || dotX == totalWidth + 1)
            return ColorIdx::BORDER;

        --dotX;
    }

    return (dotX < healthWidth) ? ColorIdx::HEALTH : (dotX < totalWidth) ? ColorIdx::DAMAGE : ColorIdx::TRANSPARENT;
};

inline ColorIdx getPlotBorder(int dotX, int totalWidth)
{
    return (0 <= dotX && dotX <= totalWidth + 1) ? ColorIdx::BORDER : ColorIdx::TRANSPARENT;
}

} // namespace

HpBar::HpBar(int bgPriority, bool hasBorder, const bn::fixed_point& topLeftPos, int height, int totalWidth,
             int healthWidth, bn::color healthColor, bn::color damageColor)
    : _height(height), _totalWidth(totalWidth), _healthWidth(healthWidth), _tiles{},
      _colors{bn::colors::black, healthColor, damageColor},
      _palette(bn::sprite_palette_item(_colors, bn::bpp_mode::BPP_4).create_palette()),
      _tilesets{
          bn::sprite_tiles_item(bn::span(_tiles.begin(), _tiles.begin() + 4), bn::bpp_mode::BPP_4).create_tiles(),
          bn::sprite_tiles_item(bn::span(_tiles.begin() + 4, _tiles.begin() + 8), bn::bpp_mode::BPP_4).create_tiles(),
          bn::sprite_tiles_item(bn::span(_tiles.begin() + 8, _tiles.begin() + 12), bn::bpp_mode::BPP_4).create_tiles(),
          bn::sprite_tiles_item(bn::span(_tiles.begin() + 12, _tiles.begin() + 16), bn::bpp_mode::BPP_4).create_tiles(),
      },
      _sprites{
          bn::sprite_ptr::create(topLeftPos + HALF_SPR_SIZE + getBorderDiff(hasBorder), SPR_SIZE, _tilesets[0],
                                 _palette),
          bn::sprite_ptr::create(topLeftPos + HALF_SPR_SIZE + getBorderDiff(hasBorder) +
                                     bn::fixed_point(SPR_SIZE.width(), 0),
                                 SPR_SIZE, _tilesets[1], _palette),
          bn::sprite_ptr::create(topLeftPos + HALF_SPR_SIZE + getBorderDiff(hasBorder) +
                                     bn::fixed_point(SPR_SIZE.width() * 2, 0),
                                 SPR_SIZE, _tilesets[2], _palette),
          bn::sprite_ptr::create(topLeftPos + HALF_SPR_SIZE + getBorderDiff(hasBorder) +
                                     bn::fixed_point(SPR_SIZE.width() * 3, 0),
                                 SPR_SIZE, _tilesets[3], _palette),
      },
      _hasBorder(hasBorder)
{
    BN_ASSERT(0 <= totalWidth && totalWidth <= SPR_CNT * SPR_SIZE.width() - ((hasBorder) ? 2 : 0),
              "Invalid totalWidth=", totalWidth, ", hasBorder=", hasBorder);
    BN_ASSERT(0 <= height && height <= SPR_SIZE.height() - ((hasBorder) ? 2 : 0), "Invalid height=", height,
              ", hasBorder=", hasBorder);

    redraw();

    for (auto& spr : _sprites)
        spr.set_bg_priority(bgPriority);
}

bool HpBar::isVisible() const
{
    return _sprites[0].visible();
}

void HpBar::setVisible(bool isVisible)
{
    for (auto& spr : _sprites)
        spr.set_visible(isVisible);
}

int HpBar::getHeight() const
{
    return _height;
}

void HpBar::setWidth(int totalWidth, int healthWidth)
{
    BN_ASSERT(0 <= healthWidth && healthWidth <= SPR_CNT * SPR_SIZE.width() - ((_hasBorder) ? 2 : 0),
              "Invalid healthWidth=", healthWidth, ", hasBorder=", _hasBorder);
    BN_ASSERT(0 <= totalWidth && totalWidth <= SPR_CNT * SPR_SIZE.width() - ((_hasBorder) ? 2 : 0),
              "Invalid totalWidth=", totalWidth, ", hasBorder=", _hasBorder);

    _totalWidth = totalWidth;
    _healthWidth = bn::min(healthWidth, totalWidth);

    redraw();
}

void HpBar::setTotalWidth(int totalWidth)
{
    BN_ASSERT(0 <= totalWidth && totalWidth <= SPR_CNT * SPR_SIZE.width() - ((_hasBorder) ? 2 : 0),
              "Invalid totalWidth=", totalWidth, ", hasBorder=", _hasBorder);

    _totalWidth = totalWidth;
    _healthWidth = bn::min(_healthWidth, totalWidth);

    redraw();
}

void HpBar::setHealthWidth(int healthWidth)
{
    BN_ASSERT(0 <= healthWidth && healthWidth <= SPR_CNT * SPR_SIZE.width() - ((_hasBorder) ? 2 : 0),
              "Invalid healthWidth=", healthWidth, ", hasBorder=", _hasBorder);

    _healthWidth = bn::min(healthWidth, _totalWidth);

    redraw();
}

auto HpBar::getTopLeftPos() const -> bn::fixed_point
{
    return _sprites[0].position() - HALF_SPR_SIZE - getBorderDiff(_hasBorder);
}

void HpBar::setTopLeftPos(const bn::fixed_point& topLeftPos)
{
    const auto diff = topLeftPos - getTopLeftPos();

    for (auto& spr : _sprites)
        spr.set_position(spr.position() + diff);
}

void HpBar::redraw()
{
    // clear tiles
    bn::memory::set_words(0, _tiles.size() * 8, &_tiles[0]);

    if (_totalWidth == 0 || _height == 0)
        return;

    for (int tileX = 0; tileX < _tiles.size(); ++tileX)
    {
        const int pX = tileX * 8;
        auto& tile = _tiles[tileX];

        // inner rows
        tile.data[_hasBorder ? 1 : 0] = (getPlotInner(pX + 0, _hasBorder, _healthWidth, _totalWidth) << 0u) |
                                        (getPlotInner(pX + 1, _hasBorder, _healthWidth, _totalWidth) << 4u) |
                                        (getPlotInner(pX + 2, _hasBorder, _healthWidth, _totalWidth) << 8u) |
                                        (getPlotInner(pX + 3, _hasBorder, _healthWidth, _totalWidth) << 12u) |
                                        (getPlotInner(pX + 4, _hasBorder, _healthWidth, _totalWidth) << 16u) |
                                        (getPlotInner(pX + 5, _hasBorder, _healthWidth, _totalWidth) << 20u) |
                                        (getPlotInner(pX + 6, _hasBorder, _healthWidth, _totalWidth) << 24u) |
                                        (getPlotInner(pX + 7, _hasBorder, _healthWidth, _totalWidth) << 28u);

        if (_height > 1)
            bn::memory::set_words(tile.data[_hasBorder ? 1 : 0], _height - 1, tile.data + (_hasBorder ? 2 : 1));

        // top & bottom border rows
        if (_hasBorder)
        {
            tile.data[0] = (getPlotBorder(pX + 0, _totalWidth) << 0u) | (getPlotBorder(pX + 1, _totalWidth) << 4u) |
                           (getPlotBorder(pX + 2, _totalWidth) << 8u) | (getPlotBorder(pX + 3, _totalWidth) << 12u) |
                           (getPlotBorder(pX + 4, _totalWidth) << 16u) | (getPlotBorder(pX + 5, _totalWidth) << 20u) |
                           (getPlotBorder(pX + 6, _totalWidth) << 24u) | (getPlotBorder(pX + 7, _totalWidth) << 28u);

            tile.data[_height + 1] = tile.data[0];
        }
    }

    for (auto& tileset : _tilesets)
        tileset.reload_tiles_ref();
}

} // namespace ut::game::bt
