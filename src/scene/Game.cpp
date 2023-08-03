#include "scene/Game.hpp"

#include <bn_display.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>

#include "asset/Bgm.hpp"
#include "core/ChoiceMsgKind.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "mtile/SpawnPoints.hpp"

namespace ut::scene
{

namespace
{
constexpr int FI_FRAMES = 12;
}

Game::Game(SceneStack& sceneStack, SceneContext& sceneContext)
    : Scene(sceneStack, sceneContext, SceneId::GAME), _worldBg(_camMngr.getCamera()), _entMngr(_gameContext),
      _gameContext{sceneContext,
                   *this,
                   sceneContext.gameState,
                   _camMngr,
                   _worldBg,
                   _entMngr,
                   _fadeMngr,
                   _roomChanger,
                   _interactStack,
                   _msg,
                   core::ChoiceMsgKind::NONE,
                   core::ChoiceMsgKind::NONE}
{
    sceneContext.menuCursorIdx = 0;
    sceneContext.gameContext = &_gameContext;

    asset::Bgm::play(sceneContext.gameState.getWorldBgm());

    _camMngr.setCamFollowEntityDiff({0, -16});

    const mtile::Warp warp({}, sceneContext.gameState.getRoom(), mtile::WarpId::INIT, false);
    _roomChanger.instantChange(warp, _gameContext);
    _worldBg.allocateGraphics();

    if (sceneContext.gameState.getRoom() == game::RoomKind::ROOM_AREA1)
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
    if (_gameContext.interactStack.top() != game::InteractState::FREE)
        return true;

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

    // Game time should be updated in battle, too.
    getContext().gameState.getTime().addTicks(1);

    return true;
}

void Game::openIngameMenu()
{
    _gameContext.interactStack.push(game::InteractState::MENU);
    reqStackPush(SceneId::INGAME_MENU);
}

void Game::openSavePrompt()
{
    _gameContext.interactStack.push(game::InteractState::INTERACT);
    reqStackPush(SceneId::SAVE_PROMPT);
}

void Game::startDialog()
{
    _gameContext.interactStack.push(game::InteractState::INTERACT);
    reqStackPush(SceneId::INGAME_DIALOG);
}

} // namespace ut::scene
