#pragma once

#include <bn_sprite_ptr.h>

namespace ut::action
{

class SpritePositionManager
{
public:
    static auto get(const bn::sprite_ptr& spr) -> const bn::fixed_point&
    {
        return spr.position();
    }

    static void set(const bn::fixed_point& pos, bn::sprite_ptr& spr)
    {
        spr.set_position(pos);
    }
};

} // namespace ut::action
