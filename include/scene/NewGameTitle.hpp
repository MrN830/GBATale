#pragma once

#include "scene/Scene.hpp"

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace ut::scene
{

class NewGameTitle : public Scene
{
public:
    NewGameTitle(SceneStack&, Context&);

    bool handleInput() override;
    bool update() override;

private:
    bn::vector<bn::sprite_ptr, 23> _texts;
};

} // namespace ut::scene
