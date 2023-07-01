#pragma once

#include "scene/Scene.hpp"

#include <bn_camera_ptr.h>

#include "game/GameContext.hpp"
#include "game/WorldBg.hpp"
#include "game/mngr/EntityManager.hpp"

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
    bn::camera_ptr _camera;
    game::WorldBg _worldBg;
    game::mngr::EntityManager _entities;

    game::GameContext _gameContext;
};

} // namespace ut::scene
