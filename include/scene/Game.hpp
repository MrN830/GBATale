#pragma once

#include "scene/Scene.hpp"

#include <bn_camera_ptr.h>

#include "game/WorldBg.hpp"

namespace ut::scene
{

class Game : public Scene
{
public:
    Game(SceneStack&, SceneContext&);

    bool handleInput() override;
    bool update() override;

private:
    bn::camera_ptr _camera;

    game::WorldBg _worldBg;
};

} // namespace ut::scene
