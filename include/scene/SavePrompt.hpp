#pragma once

#include "scene/Scene.hpp"

#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace ut::scene
{

class SavePrompt final : public Scene
{
public:
    SavePrompt(SceneStack&, SceneContext&);
    ~SavePrompt();

    bool handleInput() override;
    bool update() override;

private:
    void redrawTexts();

private:
    bn::regular_bg_ptr _bg;
    bn::sprite_ptr _cursor;

    bn::vector<bn::sprite_ptr, 15> _saveInfoTexts;
    bn::vector<bn::sprite_ptr, 3> _saveBtnText;
    bn::vector<bn::sprite_ptr, 2> _returnBtnText;

    bool _isSaveSelected = true;
    bool _isSaved = false;
};

} // namespace ut::scene
