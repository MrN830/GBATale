#pragma once

#include <bn_fixed_point.h>
#include <bn_optional.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_ptr.h>

namespace ut::asset
{
struct IPortraitInfo;
}

namespace ut::core
{

class Random;

class DialogPortrait
{
public:
    static constexpr int MAX_ANIM_FRAMES = 4;

public:
    DialogPortrait(Random& rng, int bgPriority);

    void render();

    void onWaitInput();
    void onConfirmKeyInput();

public:
    auto getInfo() const -> const asset::IPortraitInfo&;
    void setInfo(const asset::IPortraitInfo&);

    void setDialogPos(const bn::fixed_point& pos);

private:
    void startTalkAnim();
    void startBlinkAnim();
    void stopAnim();

private:
    Random& _rng;
    const int _bgPriority;

    const asset::IPortraitInfo* _info;
    bn::fixed_point _dialogPos;

    bn::optional<bn::sprite_ptr> _face;
    bn::optional<bn::sprite_ptr> _torso;
    bn::optional<bn::sprite_animate_action<MAX_ANIM_FRAMES>> _faceAnim;

    int _blinkCountdown = -1;
    bool _isWaitInput = false;
};

} // namespace ut::core
