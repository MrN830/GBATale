#pragma once

#include "scene/Scene.hpp"

#include "game/GameContext.hpp"
#include "game/sys/CameraManager.hpp"
#include "game/sys/EntityManager.hpp"
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

private:
    game::sys::CameraManager _camMngr;
    game::sys::WorldBg _worldBg;
    game::sys::EntityManager _entMngr;

    game::GameContext _gameContext;
};

} // namespace ut::scene
