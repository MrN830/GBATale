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

Game::Game(SceneStack& sceneStack, SceneContext& sceneContext)
    : Scene(sceneStack, sceneContext), _worldBg(_camMngr.getCamera()), _entMngr(_gameContext),
      _gameContext{sceneContext, sceneContext.gameState, _camMngr, _entMngr}
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

    _entMngr.reloadRoom();
    _entMngr.createFrisk({120, 60}); // test
}

Game::~Game()
{
    getContext().gameContext = nullptr;
}

bool Game::handleInput()
{
    // TODO: Disable opening menu on cutscenes, events, ...
    if (bn::keypad::start_pressed() || bn::keypad::l_pressed() || bn::keypad::r_pressed())
    {
        _gameContext.isShowingUI = true;
        reqStackPush(SceneId::INGAME_MENU);
    }

    _entMngr.handleInput();

    return true;
}

bool Game::update()
{
    _entMngr.update();
    _camMngr.update(_gameContext);
    _worldBg.render();

    return true;
}

} // namespace ut::scene
