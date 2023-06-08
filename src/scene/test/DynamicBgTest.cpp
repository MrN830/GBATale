#include "scene/test/DynamicBgTest.hpp"

#include <bn_cstring.h>
#include <bn_display.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_regular_bg_map_cell_info.h>

#include "bn_bg_palette_items_pal_mtileset_room_ruins1_bg_lower.h"
#include "bn_bg_palette_items_pal_mtileset_room_ruins1_bg_upper.h"
#include "bn_bg_palette_items_pal_mtileset_room_ruins1_bg_upper2.h"
#include "bn_regular_bg_tiles_items_mtileset_room_ruins1_bg_lower.h"
#include "bn_regular_bg_tiles_items_mtileset_room_ruins1_bg_upper.h"
#include "bn_regular_bg_tiles_items_mtileset_room_ruins1_bg_upper2.h"

namespace ut::scene::test
{

namespace
{

struct BgLayer
{
    const bn::regular_bg_tiles_item& tiles;
    const bn::bg_palette_item& palette;
};

constexpr bn::array<BgLayer, DynamicBgTest::LAYER_COUNT> LAYERS = {
    BgLayer{bn::regular_bg_tiles_items::mtileset_room_ruins1_bg_lower,
            bn::bg_palette_items::pal_mtileset_room_ruins1_bg_lower},
    BgLayer{bn::regular_bg_tiles_items::mtileset_room_ruins1_bg_upper,
            bn::bg_palette_items::pal_mtileset_room_ruins1_bg_upper},
    BgLayer{bn::regular_bg_tiles_items::mtileset_room_ruins1_bg_upper2,
            bn::bg_palette_items::pal_mtileset_room_ruins1_bg_upper2},
};

constexpr int BG_WIDTH = DynamicBgTest::CELL_COLS * 8;
constexpr int BG_HEIGHT = DynamicBgTest::CELL_ROWS * 8;

constexpr auto TOP_LEFT_ORIGIN = bn::fixed_point{-bn::display::width() / 2, -bn::display::height() / 2};
constexpr auto BG_POS = bn::fixed_point{BG_WIDTH / 2, BG_HEIGHT / 2} + TOP_LEFT_ORIGIN;

} // namespace

DynamicBgTest::DynamicBgTest(SceneStack& sceneStack, Context& context)
    : Scene(sceneStack, context), _cells{}, _bgMapItem(_cells[0], bn::size(CELL_COLS, CELL_ROWS)),
      _bgItem{
          {LAYERS[0].tiles, LAYERS[0].palette, _bgMapItem},
          {LAYERS[1].tiles, LAYERS[1].palette, _bgMapItem},
          {LAYERS[2].tiles, LAYERS[2].palette, _bgMapItem},
      },
      _bg(_bgItem[_layerIdx].create_bg(BG_POS)), _bgMap(_bg.map())
{
    swapLayer(0);
}

bool DynamicBgTest::handleInput()
{
    if (bn::keypad::l_pressed())
        swapLayer(-1);
    else if (bn::keypad::r_pressed())
        swapLayer(+1);

    constexpr int SPEED = -2;

    if (bn::keypad::up_held())
        _bg.set_y(_bg.y() - SPEED);
    if (bn::keypad::down_held())
        _bg.set_y(_bg.y() + SPEED);
    if (bn::keypad::left_held())
        _bg.set_x(_bg.x() - SPEED);
    if (bn::keypad::right_held())
        _bg.set_x(_bg.x() + SPEED);

    if (_bg.x() > BG_POS.x())
        _bg.set_x(BG_POS.x());
    if (_bg.y() > BG_POS.y())
        _bg.set_y(BG_POS.y());

    if (_bg.x() < BG_POS.x() - (BG_WIDTH - bn::display::width()))
        _bg.set_x(BG_POS.x() - (BG_WIDTH - bn::display::width()));
    if (_bg.y() < BG_POS.y() - (BG_HEIGHT - bn::display::height()))
        _bg.set_y(BG_POS.y() - (BG_HEIGHT - bn::display::height()));

    return true;
}

bool DynamicBgTest::update()
{
    return true;
}

void DynamicBgTest::swapLayer(int direction)
{
    _layerIdx = (_layerIdx + direction + LAYER_COUNT) % LAYER_COUNT;

    _bg.set_item(_bgItem[_layerIdx]);

    bn::memclear(_cells, sizeof(_cells));

    bn::regular_bg_map_cell_info cellInfo;
    for (int i = 0; i < _bgItem[_layerIdx].tiles_item().tiles_ref().size(); ++i)
    {
        const int my = i / 4 / 16, mx = i / 4 % 16;
        const int dy = (i % 4 >= 2), dx = (i % 2 == 1);

        auto& cell = _cells[_bgMapItem.cell_index(mx * 2 + dx, my * 2 + dy)];
        cellInfo.set_cell(cell);
        cellInfo.set_tile_index(i);
        cell = cellInfo.cell();
    }

    _bgMap.reload_cells_ref();
}

} // namespace ut::scene::test
