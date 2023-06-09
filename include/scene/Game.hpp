#pragma once

#include "scene/Scene.hpp"

namespace ut::scene
{

class Game : public Scene
{
public:
    Game(SceneStack&, Context&);

    bool handleInput() override;
    bool update() override;
};

} // namespace ut::scene
