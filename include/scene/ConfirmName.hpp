#pragma once

#include "scene/Scene.hpp"

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace ut::scene
{

class ConfirmName final : public Scene
{
public:
    ConfirmName(SceneStack&, Context&);
    ~ConfirmName();

    bool handleInput() override;
    bool update() override;

private:
    bn::vector<bn::sprite_ptr, 16> _tip;
    bn::vector<bn::sprite_ptr, 2> _nameSpr;
    bn::vector<bn::sprite_ptr, 2> _no;
    bn::vector<bn::sprite_ptr, 1> _yes;

    bool _isAllowed;
    bool _isYesSelected = false;
};

} // namespace ut::scene
