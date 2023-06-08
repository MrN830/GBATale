#include "game/WorldBg.hpp"

#include <bn_assert.h>
#include <bn_camera_ptr.h>
#include <bn_regular_bg_map_cell_info.h>

#include "mtile/MTilemap.hpp"
#include "util/Math.hpp"

namespace ut::game
{

namespace
{

constexpr int M_TILE_SIZE = 16;
constexpr auto M_TILE_VALID_RANGE = bn::point{16, 11};

// center of the top-left-most MCell in the room
constexpr auto BG_POS = bn::fixed_point{WorldBg::CELL_COLS * 8 / 2, WorldBg::CELL_ROWS * 8 / 2} -
                        bn::fixed_point{M_TILE_SIZE / 2, M_TILE_SIZE / 2};

} // namespace

WorldBg::WorldBg(const bn::camera_ptr& camera)
    : _camera(camera), _cellsLower{}, _cellsUpper{}, _cellsUpper2{},
      _bgMapItemLower(_cellsLower[0], bn::size(CELL_COLS, CELL_ROWS)),
      _bgMapItemUpper(_cellsUpper[0], bn::size(CELL_COLS, CELL_ROWS)),
      _bgMapItemUpper2(_cellsUpper2[0], bn::size(CELL_COLS, CELL_ROWS)), _mTilemap(nullptr), _pos(BG_POS),
      _prevCamPos(camera.position())
{
}

void WorldBg::render()
{
    redrawBg();
}

void WorldBg::redrawBg()
{
    if (isGraphicsAllocated())
    {
        if (_camera.position() != _prevCamPos)
        {
            if (redrawMCells(false))
            {
                _bgMapLower->reload_cells_ref();
                _bgMapUpper->reload_cells_ref();
                _bgMapUpper2->reload_cells_ref();
            }
        }
    }

    _prevCamPos = _camera.position();
}

void WorldBg::setMTilemap(const mtile::MTilemapBase& mTilemap)
{
    if (_mTilemap == &mTilemap)
        return;

    _mTilemap = &mTilemap;

    if (isGraphicsAllocated())
    {
        freeGraphics();
        allocateGraphics();
    }
}

bool WorldBg::isGraphicsAllocated() const
{
    return _bgLower.has_value() && _bgLower->visible();
}

void WorldBg::freeGraphics()
{
    _bgMapLower.reset();
    _bgLower.reset();
    _bgItemLower.reset();

    _bgMapUpper.reset();
    _bgUpper.reset();
    _bgItemUpper.reset();

    _bgMapUpper2.reset();
    _bgUpper2.reset();
    _bgItemUpper2.reset();
}

void WorldBg::allocateGraphics()
{
    BN_ASSERT(_mTilemap, "Tried to alloc graphics without setting MTilemap");

    _bgItemLower = bn::regular_bg_item(_mTilemap->tilesLower, _mTilemap->palLower, _bgMapItemLower);
    _bgLower = _bgItemLower->create_bg(_pos);
    _bgMapLower = _bgLower->map();

    _bgItemUpper = bn::regular_bg_item(_mTilemap->tilesUpper, _mTilemap->palUpper, _bgMapItemUpper);
    _bgUpper = _bgItemUpper->create_bg(_pos);
    _bgMapUpper = _bgUpper->map();

    _bgItemUpper2 = bn::regular_bg_item(_mTilemap->tilesUpper2, _mTilemap->palUpper2, _bgMapItemUpper2);
    _bgUpper2 = _bgItemUpper2->create_bg(_pos);
    _bgMapUpper2 = _bgUpper2->map();

    _bgLower->set_camera(_camera);
    _bgUpper->set_camera(_camera);
    _bgUpper2->set_camera(_camera);

    _bgLower->set_z_order(30);
    _bgUpper->set_z_order(20);
    _bgUpper2->set_z_order(10);

    if (redrawMCells(true))
    {
        _bgMapLower->reload_cells_ref();
        _bgMapUpper->reload_cells_ref();
        _bgMapUpper2->reload_cells_ref();
    }
}

static bn::point camPos2MCellPos(const bn::fixed_point& camPos)
{
    const auto leftTopCamPos = camPos + bn::fixed_point{M_TILE_SIZE / 2, M_TILE_SIZE / 2};
    const int x = leftTopCamPos.x().floor_integer();
    const int y = leftTopCamPos.y().floor_integer();

    return bn::point(util::div_floor(x, M_TILE_SIZE), util::div_floor(y, M_TILE_SIZE));
}

bool WorldBg::redrawMCells(const bool isRedrawAll)
{
    BN_ASSERT(_mTilemap);

    const auto prevCenter = isRedrawAll ? bn::point{0, 0} : camPos2MCellPos(_prevCamPos);
    const auto prevTopLeft = isRedrawAll ? bn::point{0, 0} : prevCenter - M_TILE_VALID_RANGE / 2;
    const auto prevBottomRight = isRedrawAll ? bn::point{0, 0} : prevCenter + M_TILE_VALID_RANGE / 2;

    const auto curCenter = camPos2MCellPos(_camera.position());
    const auto curTopLeft = curCenter - M_TILE_VALID_RANGE / 2;
    const auto curBottomRight = curCenter + M_TILE_VALID_RANGE / 2;

    bool isRedrawHappened = false;
    for (int y = curTopLeft.y(); y <= curBottomRight.y(); ++y)
    {
        for (int x = curTopLeft.x(); x <= curBottomRight.x(); ++x)
        {
            if (!isRedrawAll)
            {
                if (prevTopLeft.x() <= x && x <= prevBottomRight.x() && prevTopLeft.y() <= y &&
                    y <= prevBottomRight.y())
                    continue;
            }

            setMCell({x, y}, _mTilemap->getMTileLower(x, y), _cellsLower, _bgMapItemLower);
            setMCell({x, y}, _mTilemap->getMTileUpper(x, y), _cellsUpper, _bgMapItemUpper);
            setMCell({x, y}, _mTilemap->getMTileUpper2(x, y), _cellsUpper2, _bgMapItemUpper2);

            isRedrawHappened = true;
        }
    }

    return isRedrawHappened;
}

void WorldBg::setMCell(const bn::point& mCellPos, const mtile::MTile& mTile, bn::regular_bg_map_cell* cells,
                       const bn::regular_bg_map_item& bgMapItem)
{
    const bn::point moduloMCellPos(util::mod_floor(mCellPos.x(), 256 / M_TILE_SIZE),
                                   util::mod_floor(mCellPos.y(), 256 / M_TILE_SIZE));
    const int cellX = 2 * moduloMCellPos.x();
    const int cellY = 2 * moduloMCellPos.y();

    bn::regular_bg_map_cell_info cellInfo;
    for (int y = 0; y < 2; ++y)
    {
        for (int x = 0; x < 2; ++x)
        {
            const int ty = (mTile.vFlip ? !y : y);
            const int tx = (mTile.hFlip ? !x : x);

            auto& cell = cells[bgMapItem.cell_index(cellX + x, cellY + y)];
            cellInfo.set_cell(cell);
            cellInfo.set_tile_index(4 * mTile.mTileIdx + (2 * ty + tx));
            cellInfo.set_horizontal_flip(mTile.hFlip);
            cellInfo.set_vertical_flip(mTile.vFlip);
            cell = cellInfo.cell();
        }
    }
}

} // namespace ut::game
