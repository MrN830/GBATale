#pragma once

#include "scene/Scene.hpp"

#include <bn_regular_bg_item.h>
#include <bn_regular_bg_map_item.h>
#include <bn_regular_bg_map_ptr.h>
#include <bn_regular_bg_ptr.h>

namespace ut::scene::test
{

class DynamicBgTest : public Scene
{
public:
    DynamicBgTest(SceneStack&, Context&);

    bool handleInput() override;
    bool update() override;

private:
    void swapLayer(int direction);

public:
    static constexpr int CELL_ROWS = 256 / 8;
    static constexpr int CELL_COLS = 256 / 8;
    static constexpr int CELLS_COUNT = CELL_ROWS * CELL_COLS;

    static constexpr int LAYER_COUNT = 3;

private:
    alignas(4) bn::regular_bg_map_cell _cells[CELLS_COUNT];
    bn::regular_bg_map_item _bgMapItem;
    bn::regular_bg_item _bgItem[LAYER_COUNT];
    bn::regular_bg_ptr _bg;
    bn::regular_bg_map_ptr _bgMap;

    int _layerIdx = 0;
};

} // namespace ut::scene::test
