#include "core/SpriteShakeAction.hpp"

namespace ut::core
{

auto SpriteShakePosManager::get(const bn::sprite_ptr& spr) -> const bn::fixed_point&
{
    return spr.position();
}

void SpriteShakePosManager::set(const bn::fixed_point& pos, bn::sprite_ptr& spr)
{
    spr.set_position(pos);
}

SpriteShakeAction::SpriteShakeAction(bn::sprite_ptr& sprite, ShakeStyle shakeStyle, const bn::fixed_size& shakeScale,
                                     const bn::fixed_size& shakeDecrease, int durationUpdates)
    : ShakeValueTemplateAction(sprite, shakeStyle, shakeScale, shakeDecrease, durationUpdates)
{
}

} // namespace ut::core