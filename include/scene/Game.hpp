#pragma once

#include "scene/Scene.hpp"

#include "game/GameContext.hpp"
#include "game/sys/CameraManager.hpp"
#include "game/sys/EntityManager.hpp"
#include "game/sys/FadeManager.hpp"
#include "game/sys/RoomChanger.hpp"
#include "game/sys/WorldBg.hpp"

namespace ut::scene
{

class Game final : public Scene
{
public:
    Game(SceneStack&, SceneContext&);
    ~Game();

    bool handleInput() override;
    bool update() override;

public:
    void openIngameMenu();
    void openSavePrompt();
    void startDialog();

private:
    game::sys::CameraManager _camMngr;
    game::sys::WorldBg _worldBg;
    game::sys::EntityManager _entMngr;
    game::sys::FadeManager _fadeMngr;
    game::sys::RoomChanger _roomChanger;

    game::InteractStateStack _interactStack;

    bn::vector<bn::string_view, consts::DIALOG_MAX_LINES> _msg;

    game::GameContext _gameContext;
};

} // namespace ut::scene
