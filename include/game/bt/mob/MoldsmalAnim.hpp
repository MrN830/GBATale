#pragma once

#include "game/bt/mob/MonsterAnim.hpp"

#include <bn_sprite_actions.h>

#include "action/SpriteShakeAction.hpp"

namespace ut::game::bt::mob
{

class MoldsmalAnim final : public MonsterAnim
{
public:
    MoldsmalAnim(const bn::fixed_point& position);

    void render() override;

private:
    void startAnim(MonsterAnimKind) override;

private:
    bn::sprite_ptr _spr;
    bn::sprite_vertical_scale_loop_action _scaleAction;
    bn::sprite_move_loop_action _moveAction;

    action::SpriteShakeAction _sprShake;
};

} // namespace ut::game::bt::mob
