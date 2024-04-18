#pragma once

#include <bn_array.h>
#include <bn_colors.h>
#include <bn_sprite_palette_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_sprite_shape_size.h>
#include <bn_sprite_tiles_ptr.h>
#include <bn_tile.h>

namespace ut::game::bt
{

class HpBar
{
public:
    /// @param totalWidth [0..128] (bordered: [0..126])
    /// @param height [1..8] (bordered: [1..6])
    HpBar(int bgPriority, bool hasBorder, const bn::fixed_point& topLeftPos, int height, int totalWidth, int healthWidth,
          bn::color healthColor = bn::colors::lime, bn::color damageColor = bn::colors::red);

    bool isVisible() const;
    void setVisible(bool isVisible);

    int getHeight() const;

public:
    /// @param totalWidth [0..128] (bordered: [0..126])
    /// @param healthWidth [0..128] (bordered: [0..126])
    void setWidth(int totalWidth, int healthWidth);

    /// @param totalWidth [0..128] (bordered: [0..126])
    void setTotalWidth(int totalWidth);

    /// @param healthWidth [0..128] (bordered: [0..126])
    void setHealthWidth(int healthWidth);

public:
    auto getTopLeftPos() const -> bn::fixed_point;
    void setTopLeftPos(const bn::fixed_point& topLeftPos);

private:
    void redraw();

private:
    static constexpr int SPR_CNT = 4;
    static constexpr bn::sprite_shape_size SPR_SIZE = {32, 8};
    static constexpr bn::fixed_point HALF_SPR_SIZE = {SPR_SIZE.width() / 2, SPR_SIZE.height() / 2};

    static constexpr int TILE_CNT = SPR_CNT * SPR_SIZE.width() / 8;

private:
    const int _height;

    int _totalWidth;
    int _healthWidth;

    alignas(4) bn::array<bn::tile, TILE_CNT> _tiles;
    alignas(4) bn::array<bn::color, 16> _colors;

    bn::sprite_palette_ptr _palette;
    bn::array<bn::sprite_tiles_ptr, SPR_CNT> _tilesets;

    bn::array<bn::sprite_ptr, SPR_CNT> _sprites;

    const bool _hasBorder;
};

} // namespace ut::game::bt
