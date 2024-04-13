#pragma once

#include "core/ShakeValueTemplateAction.hpp"

#include <bn_sprite_ptr.h>

namespace ut::core
{

class SpriteShakePosManager
{
public:
    static auto get(const bn::sprite_ptr& spr) -> const bn::fixed_point&;
    static void set(const bn::fixed_point& pos, bn::sprite_ptr& spr);
};

class SpriteShakeAction : public ShakeValueTemplateAction<bn::sprite_ptr, SpriteShakePosManager>
{
public:
    SpriteShakeAction(bn::sprite_ptr& sprite, ShakeStyle shakeStyle, const bn::fixed_size& shakeScale,
                      const bn::fixed_size& shakeDecrease, int durationUpdates);
};

} // namespace ut::core