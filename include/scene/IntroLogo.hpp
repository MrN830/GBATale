#pragma once

#include "scene/Scene.hpp"

#include <bn_regular_bg_ptr.h>

namespace ut::scene
{

class IntroLogo final : public Scene
{
public:
    IntroLogo(SceneStack&, Context&);

    bool handleInput() override;
    bool update() override;

private:
    bn::regular_bg_ptr _bg;

    int _elapsedFrames = 0;
};

} // namespace ut::scene
