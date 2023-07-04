#include "scene/Game.hpp"

#include <bn_display.h>
#include <bn_dmg_music.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_music.h>

#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"

namespace ut::scene
{

static constexpr bn::fixed_point INIT_CAM_POS = {bn::display::width() / 2, bn::display::height() / 2};

Game::Game(SceneStack& sceneStack, SceneContext& sceneContext)
    : Scene(sceneStack, sceneContext), _camera(bn::camera_ptr::create(INIT_CAM_POS)), _worldBg(_camera),
      _gameContext{sceneContext, sceneContext.gameState, _camera, _entities}
{
    sceneContext.menuCursorIdx = 0;
    sceneContext.gameContext = &_gameContext;

    if (bn::music::playing())
        bn::music::stop();
    if (bn::dmg_music::playing())
        bn::dmg_music::stop();

    const auto room = sceneContext.gameState.getRoom();
    const auto mTilemap = game::getRoomMTilemap(room);

    BN_ASSERT(mTilemap != nullptr, "Invalid room=", (int)room);

    _worldBg.setMTilemap(*mTilemap);
    _worldBg.allocateGraphics();

    _entities.reloadRoom(_gameContext);
    _entities.createFrisk({120, 60}, _gameContext); // test
}

Game::~Game()
{
    getContext().gameContext = nullptr;
}

bool Game::handleInput()
{
    if (bn::keypad::start_pressed() || bn::keypad::l_pressed() || bn::keypad::r_pressed())
        reqStackPush(SceneId::INGAME_MENU);

    _entities.handleInput(_gameContext);

    return true;
}

bool Game::update()
{
    _entities.update(_gameContext);

    _worldBg.render();

    return true;
}

} // namespace ut::scene
