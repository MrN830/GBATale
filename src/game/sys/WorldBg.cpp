#include "game/sys/WorldBg.hpp"

#include <bn_assert.h>
#include <bn_blending.h>
#include <bn_camera_ptr.h>
#include <bn_display.h>
#include <bn_regular_bg_map_cell_info.h>

#include "consts.hpp"
#include "mtile/MTilemap.hpp"
#include "util/Math.hpp"

namespace ut::game::sys
{

namespace
{

constexpr int M_TILE_SIZE = 16;
constexpr int GBA_TILE_SIZE = 8;

constexpr auto GBA_TILE_VALID_RANGE = bn::point{bn::display::width() / 8, bn::display::height() / 8};

// top-left-corner of the top-left-most MCell in the room
constexpr auto BG_POS = bn::fixed_point{WorldBg::CELL_COLS * 8 / 2, WorldBg::CELL_ROWS * 8 / 2};

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
            if (redrawGBACells(false))
            {
                _bgMapLower->reload_cells_ref();
                _bgMapUpper->reload_cells_ref();
                _bgMapUpper2->reload_cells_ref();
            }
        }
    }

    _prevCamPos = _camera.position();
}

auto WorldBg::getMTilemap() const -> const mtile::MTilemapBase*
{
    return _mTilemap;
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

    _bgLower->set_priority(consts::WORLD_BG_PRIORITY);
    _bgUpper->set_priority(consts::WORLD_BG_PRIORITY);
    _bgUpper2->set_priority(consts::WORLD_BG_PRIORITY);

    _bgLower->set_z_order(30);
    _bgUpper->set_z_order(20);
    _bgUpper2->set_z_order(10);

    if (_isBlendingEnabled || bn::blending::fade_alpha() > 0)
    {
        _isBlendingEnabled = true;

        _bgLower->set_blending_enabled(true);
        _bgUpper->set_blending_enabled(true);
        _bgUpper2->set_blending_enabled(true);
    }

    if (redrawGBACells(true))
    {
        _bgMapLower->reload_cells_ref();
        _bgMapUpper->reload_cells_ref();
        _bgMapUpper2->reload_cells_ref();
    }
}

void WorldBg::setBlendingEnabled(bool isEnabled)
{
    if (_isBlendingEnabled == isEnabled)
        return;

    _isBlendingEnabled = isEnabled;

    if (isGraphicsAllocated())
    {
        _bgLower->set_blending_enabled(isEnabled);
        _bgUpper->set_blending_enabled(isEnabled);
        _bgUpper2->set_blending_enabled(isEnabled);
    }
}

static bn::point camPos2GBACellPos(const bn::fixed_point& camPos)
{
    const int x = camPos.x().floor_integer();
    const int y = camPos.y().floor_integer();

    return bn::point(util::div_floor(x, GBA_TILE_SIZE), util::div_floor(y, GBA_TILE_SIZE));
}

bool WorldBg::redrawGBACells(const bool isRedrawAll)
{
    BN_ASSERT(_mTilemap);

    const auto prevCenter = isRedrawAll ? bn::point{0, 0} : camPos2GBACellPos(_prevCamPos);
    const auto prevTopLeft = isRedrawAll ? bn::point{0, 0} : prevCenter - GBA_TILE_VALID_RANGE / 2;
    const auto prevBottomRight = isRedrawAll ? bn::point{0, 0} : prevCenter + GBA_TILE_VALID_RANGE / 2;

    const auto curCenter = camPos2GBACellPos(_camera.position());
    const auto curTopLeft = curCenter - GBA_TILE_VALID_RANGE / 2;
    const auto curBottomRight = curCenter + GBA_TILE_VALID_RANGE / 2;

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

            setGBACell({x, y}, _mTilemap->getMTileLower(util::div_floor(x, 2), util::div_floor(y, 2)), _cellsLower,
                       _bgMapItemLower);
            setGBACell({x, y}, _mTilemap->getMTileUpper(util::div_floor(x, 2), util::div_floor(y, 2)), _cellsUpper,
                       _bgMapItemUpper);
            setGBACell({x, y}, _mTilemap->getMTileUpper2(util::div_floor(x, 2), util::div_floor(y, 2)), _cellsUpper2,
                       _bgMapItemUpper2);

            isRedrawHappened = true;
        }
    }

    return isRedrawHappened;
}

void WorldBg::setGBACell(const bn::point& gbaCellPos, const mtile::MTile& mTile, bn::regular_bg_map_cell* cells,
                         const bn::regular_bg_map_item& bgMapItem)
{
    const int cellX = util::mod_floor(gbaCellPos.x(), 256 / GBA_TILE_SIZE);
    const int cellY = util::mod_floor(gbaCellPos.y(), 256 / GBA_TILE_SIZE);

    int tileY = (cellY % 2 == 0 ? 0 : 1);
    int tileX = (cellX % 2 == 0 ? 0 : 1);
    tileY = (mTile.vFlip ? !tileY : tileY);
    tileX = (mTile.hFlip ? !tileX : tileX);

    auto& cell = cells[bgMapItem.cell_index(cellX, cellY)];

    bn::regular_bg_map_cell_info cellInfo(cell);
    cellInfo.set_tile_index(4 * mTile.mTileIdx + (2 * tileY + tileX));
    cellInfo.set_horizontal_flip(mTile.hFlip);
    cellInfo.set_vertical_flip(mTile.vFlip);
    cell = cellInfo.cell();
}

} // namespace ut::game::sys
