#pragma once

#include "scene/Scene.hpp"

#include <bn_array.h>
#include <bn_blending_actions.h>
#include <bn_fixed_rect.h>
#include <bn_optional.h>
#include <bn_regular_bg_actions.h>
#include <bn_regular_bg_ptr.h>
#include <bn_vector.h>

#include "core/DialogWriter.hpp"

namespace ut::scene
{

class IntroStory final : public Scene
{
public:
    IntroStory(SceneStack&, SceneContext&);
    ~IntroStory();

    bool handleInput() override;
    bool update() override;

private:
    bool isSkipRequested();

    void updateBgFade();
    void startNextBgFade();

    void updateBgMove();

private:
    const bn::fixed_rect _prevWindowRect;

    bn::optional<bn::regular_bg_ptr> _bg;
    bn::blending_transparency_alpha_to_action _bgFade;
    bn::optional<bn::regular_bg_move_to_action> _bgMove;

    bn::array<bn::string_view, 7> _dialogs;
    bn::vector<bn::sprite_ptr, 17> _texts;
    core::DialogWriter _dialogWriter;

    int _elapsedFrames = 0;
    int _introBgIdx0 = 0;
    int _skipCountdown = -1;
    bool _isFadeOut = false;
    bool _isMtEbottPosMoved = false;
};

} // namespace ut::scene
