// SPDX-FileCopyrightText: Copyright 2024 copyrat90
// SPDX-License-Identifier: 0BSD

#include "core/BgBox.hpp"

#include <bn_memory.h>
#include <bn_profiler.h>
#include <bn_regular_bg_map_cell_info.h>

#ifdef UT_BG_BOX_PROFILER_ENABLED

#ifdef UT_BG_BOX_DEBUG
#define UT_BG_BOX_PROFILER_START(id) \
    do \
    { \
        if (!_debug) \
            BN_PROFILER_START(id); \
    } while (false)
#define UT_BG_BOX_PROFILER_STOP() \
    do \
    { \
        if (!_debug) \
            BN_PROFILER_STOP(); \
    } while (false)
#else // ! UT_BG_BOX_DEBUG
#define UT_BG_BOX_PROFILER_START(id) \
    do \
    { \
        BN_PROFILER_START(id); \
    } while (false)
#define UT_BG_BOX_PROFILER_STOP() \
    do \
    { \
        BN_PROFILER_STOP(); \
    } while (false)
#endif // UT_BG_BOX_DEBUG

#else // ! UT_BG_BOX_PROFILER_ENABLED

#define UT_BG_BOX_PROFILER_START(id) \
    do \
    { \
    } while (false)
#define UT_BG_BOX_PROFILER_STOP() \
    do \
    { \
    } while (false)

#endif // UT_BG_BOX_PROFILER_ENABLED

namespace ut::core
{

namespace
{

BN_CODE_IWRAM inline uint8_t getPlotColor(int dotX, int dotY, const bn::top_left_rect& borderRect,
                                          const bn::top_left_rect& fillRect, int fillColorIdx, int borderColorIdx)
{
    const bn::point dotPos(dotX, dotY);

    if ((fillRect.left() <= dotPos.x() && dotPos.x() < fillRect.right()) &&
        (fillRect.top() <= dotPos.y() && dotPos.y() < fillRect.bottom()))
        return fillColorIdx;
    if ((borderRect.left() <= dotPos.x() && dotPos.x() < borderRect.right()) &&
        (borderRect.top() <= dotPos.y() && dotPos.y() < borderRect.bottom()))
        return borderColorIdx;
    return 0;
}

} // namespace

BN_CODE_IWRAM void BgBox::redraw()
{
    UT_BG_BOX_PROFILER_START("map: clear");
    // clear map
    BN_ASSERT(sizeof(_cells) % 4 == 0);
    bn::memory::set_words(0, sizeof(_cells) / 4, _cells);
    UT_BG_BOX_PROFILER_STOP();

    UT_BG_BOX_PROFILER_START("used tile pos: clear");
    // clear `_usedTilePos`
    BN_ASSERT(sizeof(_usedTilePos) % 4 == 0);
    bn::memory::set_words(-1, sizeof(_usedTilePos) / 4, _usedTilePos);
    UT_BG_BOX_PROFILER_STOP();

    UT_BG_BOX_PROFILER_START("map: get rects");
    const auto borderRect = convertToPositiveIntRect(getClampedRect());
    const auto fillRect = bn::top_left_rect{
        borderRect.x() + _borderThickness,
        borderRect.y() + _borderThickness,
        bn::max(0, borderRect.width() - 2 * _borderThickness),
        bn::max(0, borderRect.height() - 2 * _borderThickness),
    };
    UT_BG_BOX_PROFILER_STOP();

    UT_BG_BOX_PROFILER_START("map: draw");
    // map: draw border
    int xLo = borderRect.left() / TILE_LEN;
    int yLo = borderRect.top() / TILE_LEN;
    int xHi = (borderRect.right() - 1) / TILE_LEN;
    int yHi = (borderRect.bottom() - 1) / TILE_LEN;

    // map: draw border sides
    drawMapSides(true, xLo, xHi, yLo, yHi);

    // map: draw fill
    xLo = fillRect.left() / TILE_LEN;
    yLo = fillRect.top() / TILE_LEN;
    xHi = (fillRect.right() - 1) / TILE_LEN;
    yHi = (fillRect.bottom() - 1) / TILE_LEN;

    // map: draw fill sides
    drawMapSides(false, xLo, xHi, yLo, yHi);

    // map: draw fill mid
    for (int y = yLo + 1; y <= yHi - 1; ++y)
        setCellLine(xLo + 1, xHi - 1, y, TileIdx::MID_INNER);
    UT_BG_BOX_PROFILER_STOP();

    // plot dots in each unique tile
    UT_BG_BOX_PROFILER_START("tiles: plot");

#ifdef UT_BG_BOX_DEBUG
    if (!_debug)
#endif
    {
        // `EMPTY`, `MID_INNER` tiles are not updated
        for (int i = TileIdx::MID_INNER + 1; i < UNIQUE_TILE_COUNT; ++i)
        {
            const auto& tilePos = _usedTilePos[i];
            if (tilePos.x < 0)
                continue;

            auto& tile = _tiles[i];
            for (int y = 0; y < TILE_LEN; ++y)
            {
                const int pY = tilePos.y * TILE_LEN + y;
                const int pX = tilePos.x * TILE_LEN;

                tile.data[y] = (getPlotColor(pX + 0, pY, borderRect, fillRect, _fillColorIdx, _borderColorIdx) << 0u) |
                               (getPlotColor(pX + 1, pY, borderRect, fillRect, _fillColorIdx, _borderColorIdx) << 4u) |
                               (getPlotColor(pX + 2, pY, borderRect, fillRect, _fillColorIdx, _borderColorIdx) << 8u) |
                               (getPlotColor(pX + 3, pY, borderRect, fillRect, _fillColorIdx, _borderColorIdx) << 12u) |
                               (getPlotColor(pX + 4, pY, borderRect, fillRect, _fillColorIdx, _borderColorIdx) << 16u) |
                               (getPlotColor(pX + 5, pY, borderRect, fillRect, _fillColorIdx, _borderColorIdx) << 20u) |
                               (getPlotColor(pX + 6, pY, borderRect, fillRect, _fillColorIdx, _borderColorIdx) << 24u) |
                               (getPlotColor(pX + 7, pY, borderRect, fillRect, _fillColorIdx, _borderColorIdx) << 28u);
            }
        }
    }
    UT_BG_BOX_PROFILER_STOP();

    _map.reload_cells_ref();

#ifdef UT_BG_BOX_DEBUG
    if (!_debug)
#endif
        _tileset.reload_tiles_ref();
}

BN_CODE_IWRAM void BgBox::setCell(int x, int y, int tileIdx)
{
    bn::regular_bg_map_cell& cell = _cells[_mapItem.cell_index(x, y)];
    bn::regular_bg_map_cell_info cellInfo(cell);
    cellInfo.set_tile_index(tileIdx);
    cell = cellInfo.cell();

    // cache an used tile pos
    _usedTilePos[tileIdx] = {(int8_t)x, (int8_t)y};
}

BN_CODE_IWRAM void BgBox::setCellLine(int xLo, int xHi, int y, int tileIdx)
{
    const int cellCount = xHi - xLo + 1;
    if (cellCount <= 0)
        return;

    bn::regular_bg_map_cell& startCell = _cells[_mapItem.cell_index(xLo, y)];
    bn::regular_bg_map_cell_info startCellInfo(startCell);
    startCellInfo.set_tile_index(tileIdx);
    bn::memory::set_half_words(startCellInfo.cell(), cellCount, &startCell);

    // cache an used tile pos
    _usedTilePos[tileIdx] = {(int8_t)xHi, (int8_t)y};
}

BN_CODE_IWRAM void BgBox::drawMapSides(bool isOuter, int xLo, int xHi, int yLo, int yHi)
{
    // sides
    setCellLine(xLo + 1, xHi - 1, yLo, isOuter ? TileIdx::TOP_OUTER : TileIdx::TOP_INNER);
    setCellLine(xLo + 1, xHi - 1, yHi, isOuter ? TileIdx::BOTTOM_OUTER : TileIdx::BOTTOM_INNER);

    for (int y = yLo + 1; y <= yHi - 1; ++y)
    {
        setCell(xLo, y, isOuter ? TileIdx::LEFT_OUTER : TileIdx::LEFT_INNER);
        setCell(xHi, y, isOuter ? TileIdx::RIGHT_OUTER : TileIdx::RIGHT_INNER);
    }

    // corners
    setCell(xLo, yLo, isOuter ? TileIdx::TOP_LEFT_OUTER : TileIdx::TOP_LEFT_INNER);
    setCell(xHi, yLo, isOuter ? TileIdx::TOP_RIGHT_OUTER : TileIdx::TOP_RIGHT_INNER);
    setCell(xLo, yHi, isOuter ? TileIdx::BOTTOM_LEFT_OUTER : TileIdx::BOTTOM_LEFT_INNER);
    setCell(xHi, yHi, isOuter ? TileIdx::BOTTOM_RIGHT_OUTER : TileIdx::BOTTOM_RIGHT_INNER);
}

} // namespace ut::core
