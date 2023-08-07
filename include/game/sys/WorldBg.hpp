#pragma once

#include <bn_fixed_point.h>
#include <bn_regular_bg_item.h>
#include <bn_regular_bg_map_item.h>
#include <bn_regular_bg_map_ptr.h>
#include <bn_regular_bg_ptr.h>

namespace bn
{
class camera_ptr;
} // namespace bn

namespace ut::mtile
{
struct MTile;
struct MTilemapBase;
} // namespace ut::mtile

namespace ut::game::sys
{

class WorldBg final
{
public:
    static constexpr int CELL_ROWS = 256 / 8;
    static constexpr int CELL_COLS = 256 / 8;
    static constexpr int CELLS_COUNT = CELL_ROWS * CELL_COLS;

public:
    WorldBg(const bn::camera_ptr&);

    void render();
    void redrawBg();

    auto getMTilemap() const -> const mtile::MTilemapBase*;
    void setMTilemap(const mtile::MTilemapBase&);

    bool isGraphicsAllocated() const;
    void freeGraphics();
    void allocateGraphics();

    void setBlendingEnabled(bool isEnabled);

private:
    /**
     * @brief Redraw GBA cells (half meta-cells)
     *
     * @param `isRedrawAll` Ignore the previous loaded gba cells and redraw the whole window.
     * @return `bool` whether or not the redraw happened
     */
    bool redrawGBACells(bool isRedrawAll);

    void setGBACell(const bn::point& mCellPos, const mtile::MTile& mTile, bn::regular_bg_map_cell* cells,
                    const bn::regular_bg_map_item& bgMapItem);

private:
    const bn::camera_ptr& _camera;

    alignas(4) bn::regular_bg_map_cell _cellsLower[CELLS_COUNT];
    alignas(4) bn::regular_bg_map_cell _cellsUpper[CELLS_COUNT];
    alignas(4) bn::regular_bg_map_cell _cellsUpper2[CELLS_COUNT];

    bn::regular_bg_map_item _bgMapItemLower;
    bn::optional<bn::regular_bg_item> _bgItemLower;
    bn::optional<bn::regular_bg_ptr> _bgLower;
    bn::optional<bn::regular_bg_map_ptr> _bgMapLower;

    bn::regular_bg_map_item _bgMapItemUpper;
    bn::optional<bn::regular_bg_item> _bgItemUpper;
    bn::optional<bn::regular_bg_ptr> _bgUpper;
    bn::optional<bn::regular_bg_map_ptr> _bgMapUpper;

    bn::regular_bg_map_item _bgMapItemUpper2;
    bn::optional<bn::regular_bg_item> _bgItemUpper2;
    bn::optional<bn::regular_bg_ptr> _bgUpper2;
    bn::optional<bn::regular_bg_map_ptr> _bgMapUpper2;

    const mtile::MTilemapBase* _mTilemap;

    bool _isBlendingEnabled = false;

    bn::fixed_point _pos;
    bn::fixed_point _prevCamPos;
};

} // namespace ut::game::sys
