#include "scene/test/WorldBgTest.hpp"

#include <bn_display.h>
#include <bn_keypad.h>

#include "asset/Bgm.hpp"
#include "game/RoomInfo.hpp"
#include "mtile/MTilemap.hpp"

#include "common_variable_8x16_sprite_font.h"

namespace ut::scene::test
{

static constexpr bn::fixed_point INIT_CAM_POS = {bn::display::width() / 2, bn::display::height() / 2};

static constexpr bn::string_view INFO_TEXT_LINES[] = {
    "PAD: Move",
    "A: Toggle fast/slow move",
    "L/R: See other rooms",
};

WorldBgTest::WorldBgTest(SceneStack& sceneStack, SceneContext& context)
    : Scene(sceneStack, context, SceneId::WORLD_BG_TEST), _camera(bn::camera_ptr::create(INIT_CAM_POS)),
      _worldBg(_camera), _infoTextGen(common::variable_8x16_sprite_font),
      _info("WorldBgTest", INFO_TEXT_LINES, _infoTextGen)
{
    asset::Bgm::stop();

    _curRoom = game::RoomKind::ROOM_AREA1;
    _worldBg.setMTilemap(*game::getRoomMTilemap(_curRoom));
    _worldBg.allocateGraphics();
}

bool WorldBgTest::handleInput()
{
    _info.update();

    if ((bn::keypad::l_pressed() || bn::keypad::r_pressed()) && !(bn::keypad::l_pressed() && bn::keypad::r_pressed()))
    {
        constexpr int TOTAL_ROOMS = (int)game::RoomKind::TOTAL_COUNT;

        const int direction = (bn::keypad::l_pressed() ? -1 : +1);
        auto nextRoom = game::RoomKind(((int)_curRoom + direction + TOTAL_ROOMS) % TOTAL_ROOMS);

        while (nullptr == game::getRoomMTilemap(nextRoom))
            nextRoom = game::RoomKind(((int)nextRoom + direction + TOTAL_ROOMS) % TOTAL_ROOMS);

        _camera.set_position(INIT_CAM_POS);
        _worldBg.setMTilemap(*game::getRoomMTilemap(nextRoom));
        _curRoom = nextRoom;
    }
    else
    {
        if (bn::keypad::a_pressed())
            _fastMove = !_fastMove;

        constexpr int SPEED = 4;
        const int speed = SPEED * (_fastMove ? 2 : 1);

        if (bn::keypad::up_held())
            _camera.set_y(_camera.y() - speed);
        if (bn::keypad::down_held())
            _camera.set_y(_camera.y() + speed);
        if (bn::keypad::left_held())
            _camera.set_x(_camera.x() - speed);
        if (bn::keypad::right_held())
            _camera.set_x(_camera.x() + speed);
    }

    snapCamToRoom();

    return true;
}

bool WorldBgTest::update()
{
    _worldBg.render();

    return true;
}

void WorldBgTest::snapCamToRoom()
{
    const auto& mTilemap = *_worldBg.getMTilemap();
    const bn::fixed_point roomBottomRight = {mTilemap.getWidth() * 16 - INIT_CAM_POS.x(),
                                             mTilemap.getHeight() * 16 - INIT_CAM_POS.y()};

    if (mTilemap.getWidth() <= bn::display::width() / 16)
        _camera.set_x(mTilemap.getWidth() * 16 / 2);
    else if (_camera.x() < INIT_CAM_POS.x())
        _camera.set_x(INIT_CAM_POS.x());
    else if (_camera.x() > roomBottomRight.x())
        _camera.set_x(roomBottomRight.x());

    if (mTilemap.getHeight() <= bn::display::height() / 16)
        _camera.set_y(mTilemap.getHeight() * 16 / 2);
    else if (_camera.y() < INIT_CAM_POS.y())
        _camera.set_y(INIT_CAM_POS.y());
    else if (_camera.y() > roomBottomRight.y())
        _camera.set_y(roomBottomRight.y());
}

} // namespace ut::scene::test
