#pragma once

#include "scene/Scene.hpp"

namespace ut::scene
{

class Intro final : public Scene
{
public:
    Intro(SceneStack&, SceneContext&);

    bool handleInput() override;
    bool update() override;
};

} // namespace ut::scene
