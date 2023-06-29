#pragma once

#include "scene/Scene.hpp"

#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace ut::scene
{

class Title final : public Scene
{
public:
    Title(SceneStack&, SceneContext&);

    bool handleInput() override;
    bool update() override;

private:
    bn::regular_bg_ptr _bg;
    bn::vector<bn::sprite_ptr, 15> _saveInfoTexts;
    bn::vector<bn::sprite_ptr, 2> _continueBtnText;
    bn::vector<bn::sprite_ptr, 2> _resetBtnText;

    bool _isContinueSelected;
};

} // namespace ut::scene
