#pragma once

#include "scene/Scene.hpp"

namespace ut::scene
{

class Intro final : public Scene
{
public:
    Intro(SceneStack&, Context&);

    bool handleInput() override;
    bool update() override;
};

} // namespace ut::scene
