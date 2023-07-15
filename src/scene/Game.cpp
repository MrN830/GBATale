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

namespace
{
constexpr int FI_FRAMES = 12;
}

Game::Game(SceneStack& sceneStack, SceneContext& sceneContext)
    : Scene(sceneStack, sceneContext), _worldBg(_camMngr.getCamera()), _entMngr(_gameContext),
      _gameContext{sceneContext, sceneContext.gameState, _camMngr, _worldBg, _entMngr, _fadeMngr, _roomChanger}
{
    sceneContext.menuCursorIdx = 0;
    sceneContext.gameContext = &_gameContext;

    if (bn::music::playing())
        bn::music::stop();
    if (bn::dmg_music::playing())
        bn::dmg_music::stop();

    const auto room = sceneContext.gameState.getRoom();
    _roomChanger.instantChange(room, mtile::WarpId::INIT, _gameContext);
    _worldBg.allocateGraphics();

    if (sceneContext.gameState.getRSavedCount() <= 0)
    {
        bn::blending::set_fade_alpha(1);
        _fadeMngr.startFadeIn(FI_FRAMES);
    }
}

Game::~Game()
{
    getContext().gameContext = nullptr;
}

bool Game::handleInput()
{
    if (_roomChanger.isChanging())
        return true;

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
    _roomChanger.update(_gameContext);
    _entMngr.update();
    _camMngr.update(_gameContext);
    _worldBg.render();
    _fadeMngr.render();

    return true;
}

} // namespace ut::scene
