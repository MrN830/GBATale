#pragma once

#include <bn_array.h>
#include <bn_colors.h>
#include <bn_sprite_palette_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_tiles_ptr.h>
#include <bn_tile.h>

namespace ut::game::bt
{

class HpBar
{
public:
    /// @param totalWidth [0..64]
    /// @param height [1..8]
    HpBar(int bgPriority, const bn::fixed_point& topLeftPos, int height, int totalWidth, int healthWidth,
          bn::color healthColor = bn::colors::lime, bn::color damageColor = bn::colors::red);

    void setVisible(bool isVisible);

public:
    /// @param totalWidth [0..64]
    /// @param healthWidth [0..64]
    void setWidth(int totalWidth, int healthWidth);

    /// @param totalWidth [0..64]
    void setTotalWidth(int totalWidth);

    /// @param healthWidth [0..64]
    void setHealthWidth(int healthWidth);

public:
    auto getTopLeftPos() const -> bn::fixed_point;
    void setTopLeftPos(const bn::fixed_point& topLeftPos);

private:
    void redraw();

private:
    const int _height;

    int _totalWidth;
    int _healthWidth;

    alignas(4) bn::array<bn::tile, 8> _tiles;
    alignas(4) bn::array<bn::color, 16> _colors;

    bn::sprite_palette_ptr _palette;
    bn::array<bn::sprite_tiles_ptr, 2> _tilesets;

    bn::array<bn::sprite_ptr, 2> _sprites;
};

} // namespace ut::game::bt
