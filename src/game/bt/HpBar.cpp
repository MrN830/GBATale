#include "game/bt/HpBar.hpp"

#include <bn_memory.h>
#include <bn_sprite_palette_item.h>
#include <bn_sprite_shape_size.h>
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
};

constexpr bn::sprite_shape_size SPR_SIZE = {32, 8};

constexpr bn::fixed_point HALF_SPR_SIZE = {SPR_SIZE.width() / 2, SPR_SIZE.height() / 2};

inline ColorIdx getPlotColor(int dotX, int healthWidth, int totalWidth)
{
    return (dotX < healthWidth) ? ColorIdx::HEALTH : (dotX < totalWidth) ? ColorIdx::DAMAGE : ColorIdx::TRANSPARENT;
};

} // namespace

HpBar::HpBar(int bgPriority, const bn::fixed_point& topLeftPos, int height, int totalWidth, int healthWidth,
             bn::color healthColor, bn::color damageColor)
    : _height(height), _totalWidth(totalWidth), _healthWidth(healthWidth), _tiles{},
      _colors{bn::colors::black, healthColor, damageColor},
      _palette(bn::sprite_palette_item(_colors, bn::bpp_mode::BPP_4).create_palette()),
      _tilesets{
          bn::sprite_tiles_item(bn::span(_tiles.begin(), _tiles.begin() + 4), bn::bpp_mode::BPP_4).create_tiles(),
          bn::sprite_tiles_item(bn::span(_tiles.begin() + 4, _tiles.begin() + 8), bn::bpp_mode::BPP_4).create_tiles(),
      },
      _sprites{
          bn::sprite_ptr::create(topLeftPos + HALF_SPR_SIZE, SPR_SIZE, _tilesets[0], _palette),
          bn::sprite_ptr::create(topLeftPos + HALF_SPR_SIZE + bn::fixed_point(SPR_SIZE.width(), 0), SPR_SIZE,
                                 _tilesets[1], _palette),
      }
{
    BN_ASSERT(0 <= totalWidth && totalWidth <= 64, "Invalid totalWidth=", totalWidth);
    BN_ASSERT(0 <= height && height <= 8, "Invalid height=", height);

    redraw();

    for (auto& spr : _sprites)
        spr.set_bg_priority(bgPriority);
}

void HpBar::setVisible(bool isVisible)
{
    for (auto& spr : _sprites)
        spr.set_visible(isVisible);
}

void HpBar::setWidth(int totalWidth, int healthWidth)
{
    BN_ASSERT(0 <= healthWidth && healthWidth <= 64, "Invalid healthWidth=", healthWidth);
    BN_ASSERT(0 <= totalWidth && totalWidth <= 64, "Invalid totalWidth=", totalWidth);

    _totalWidth = totalWidth;
    _healthWidth = bn::min(healthWidth, totalWidth);

    redraw();
}

void HpBar::setTotalWidth(int totalWidth)
{
    BN_ASSERT(0 <= totalWidth && totalWidth <= 64, "Invalid totalWidth=", totalWidth);

    _totalWidth = totalWidth;
    _healthWidth = bn::min(_healthWidth, totalWidth);

    redraw();
}

void HpBar::setHealthWidth(int healthWidth)
{
    BN_ASSERT(0 <= healthWidth && healthWidth <= 64, "Invalid healthWidth=", healthWidth);

    _healthWidth = bn::min(healthWidth, _totalWidth);

    redraw();
}

auto HpBar::getTopLeftPos() const -> bn::fixed_point
{
    return _sprites[0].position() - HALF_SPR_SIZE;
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

        tile.data[0] = (getPlotColor(pX + 0, _healthWidth, _totalWidth) << 0u) |
                       (getPlotColor(pX + 1, _healthWidth, _totalWidth) << 4u) |
                       (getPlotColor(pX + 2, _healthWidth, _totalWidth) << 8u) |
                       (getPlotColor(pX + 3, _healthWidth, _totalWidth) << 12u) |
                       (getPlotColor(pX + 4, _healthWidth, _totalWidth) << 16u) |
                       (getPlotColor(pX + 5, _healthWidth, _totalWidth) << 20u) |
                       (getPlotColor(pX + 6, _healthWidth, _totalWidth) << 24u) |
                       (getPlotColor(pX + 7, _healthWidth, _totalWidth) << 28u);

        if (_height > 1)
            bn::memory::set_words(tile.data[0], _height - 1, tile.data + 1);
    }

    for (auto& tileset : _tilesets)
        tileset.reload_tiles_ref();
}

} // namespace ut::game::bt
