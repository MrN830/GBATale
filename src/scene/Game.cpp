#include "scene/Game.hpp"

#include <bn_keypad.h>
#include <bn_music.h>

#include "room_ruins1.hpp" // test

namespace ut::scene
{

Game::Game(SceneStack& sceneStack, Context& context)
    : Scene(sceneStack, context), _camera(bn::camera_ptr::create(0, 0)), _worldBg(_camera)
{
    if (bn::music::playing())
        bn::music::stop();

    _worldBg.setMTilemap(mtile::room_ruins1); // test
    _worldBg.allocateGraphics();
}

bool Game::handleInput()
{
    constexpr int SPEED = 4;

    if (bn::keypad::up_held())
        _camera.set_y(_camera.y() - SPEED);
    if (bn::keypad::down_held())
        _camera.set_y(_camera.y() + SPEED);
    if (bn::keypad::left_held())
        _camera.set_x(_camera.x() - SPEED);
    if (bn::keypad::right_held())
        _camera.set_x(_camera.x() + SPEED);

    return true;
}

bool Game::update()
{
    _worldBg.render();

    return true;
}

} // namespace ut::scene
