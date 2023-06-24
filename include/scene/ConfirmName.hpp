#pragma once

#include "scene/Scene.hpp"

#include <bn_blending_actions.h>
#include <bn_sprite_actions.h>
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
    bool isConfirmed() const;

private:
    bn::blending_fade_alpha_to_action _whiteOut;

    bn::vector<bn::sprite_ptr, 16> _tip;
    bn::vector<bn::sprite_ptr, 4> _nameSpr;
    bn::vector<bn::sprite_ptr, 2> _no;
    bn::vector<bn::sprite_ptr, 1> _yes;

    bn::vector<bn::sprite_move_to_action, 4> _nameMoveAction;
    bn::vector<bn::sprite_scale_to_action, 4> _nameScaleAction;

    int _gameSceneCountdown = -1;
    bool _isAllowed;
    bool _isYesSelected = false;
};

} // namespace ut::scene
