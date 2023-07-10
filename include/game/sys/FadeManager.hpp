#pragma once

#include <bn_blending_actions.h>
#include <bn_optional.h>

namespace ut::game::sys
{

class FadeManager final
{
public:
    FadeManager();

    void render();

public:
    bool isFadeOut() const;
    bool isDone() const;

    void startFadeIn(int durationUpdates);
    void startFadeOut(int durationUpdates);

private:
    bn::optional<bn::blending_fade_alpha_to_action> _fade;
    bool _isFadeOut = false;
};

} // namespace ut::game::sys
