#pragma once

#include "scene/Scene.hpp"

namespace ut::scene::test
{

class SaveTest : public Scene
{
public:
    SaveTest(SceneStack&, SceneContext&);

    bool handleInput() override;
    bool update() override;
};

} // namespace ut::scene::test
