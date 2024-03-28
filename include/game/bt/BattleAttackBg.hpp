#pragma once

#include <bn_affine_bg_actions.h>
#include <bn_affine_bg_ptr.h>
#include <bn_blending_actions.h>
#include <bn_optional.h>

namespace ut::game::bt
{

class BattleAttackBg
{
public:
    void update();

public:
    bool isVisible() const;
    void setVisible(bool isVisible);

    // This calls `setVisible(false)` when fade out is done
    void startFadeOut();

private:
    bn::optional<bn::affine_bg_ptr> _bg;

    bn::optional<bn::affine_bg_horizontal_scale_to_action> _shrink;
    bn::optional<bn::blending_transparency_alpha_to_action> _fade;
};

} // namespace ut::game::bt
