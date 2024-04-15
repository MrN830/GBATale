#pragma once

#include "action/ShakeValueTemplateAction.hpp"

#include "action/SpritePositionManager.hpp"

namespace ut::action
{

class SpriteShakeAction : public ShakeValueTemplateAction<bn::sprite_ptr, SpritePositionManager>
{
public:
    SpriteShakeAction(bn::sprite_ptr& sprite, ShakeStyle shakeStyle, const bn::fixed_size& shakeScale,
                      const bn::fixed_size& shakeDecrease, int durationUpdates)
        : ShakeValueTemplateAction(sprite, shakeStyle, shakeScale, shakeDecrease, durationUpdates)
    {
    }
};

} // namespace ut::action
