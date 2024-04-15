#pragma once

#include "game/bt/mob/MonsterAnim.hpp"

#include <bn_sprite_animate_actions.h>

#include "action/SpritePathLoopAction.hpp"
#include "action/SpriteShakeAction.hpp"

namespace ut::game::bt::mob
{

class FroggitAnim final : public MonsterAnim
{
public:
    FroggitAnim(const bn::fixed_point& position);

    void render() override;

private:
    void startAnim(MonsterAnimKind) override;

private:
    bn::fixed_point _initPos;

    bn::sprite_ptr _legs, _head, _hurt;
    bn::sprite_animate_action<2> _legsAnim, _headAnim;

    action::SpriteShakeAction _hurtShake;
    action::SpritePathLoopAction _headMove;
};

} // namespace ut::game::bt::mob
