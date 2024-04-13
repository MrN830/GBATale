#pragma once

#include "game/bt/mob/MonsterAnim.hpp"

#include <bn_sprite_actions.h>
#include <bn_sprite_animate_actions.h>
#include <bn_sprite_ptr.h>

#include "core/SpriteShakeAction.hpp"

namespace ut::game::bt::mob
{

class WhimsunAnim final : public MonsterAnim
{
public:
    WhimsunAnim(const bn::fixed_point& position);

    void render() override;

private:
    void startAnim(MonsterAnimKind) override;

private:
    bn::sprite_ptr _spr;
    bn::sprite_animate_action<2> _sprAnim;
    bn::sprite_move_loop_action _sprMoveUpDown;

    core::SpriteShakeAction _sprShake;
};

} // namespace ut::game::bt::mob
