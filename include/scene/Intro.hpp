#pragma once

#include "scene/Scene.hpp"

#include <bn_blending_actions.h>
#include <bn_fixed_rect.h>
#include <bn_optional.h>
#include <bn_regular_bg_actions.h>
#include <bn_regular_bg_ptr.h>

namespace ut::scene
{

class Intro final : public Scene
{
public:
    Intro(SceneStack&, Context&);
    ~Intro();

    bool handleInput() override;
    bool update() override;

private:
    void updateBgFade();
    void startNextBgFade();

    void updateBgMove();

private:
    const bn::fixed_rect _prevWindowRect;

    bn::regular_bg_ptr _bg;
    bn::blending_transparency_alpha_to_action _bgFade;
    bn::optional<bn::regular_bg_move_to_action> _bgMove;

    int _elapsedFrames = 0;
    int _introBgIdx0 = 0;
    bool _isFadeOut = false;
};

} // namespace ut::scene
