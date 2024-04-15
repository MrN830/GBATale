#include "game/bt/mob/FroggitAnim.hpp"

#include "asset/Path.hpp"

#include "bn_sprite_items_bt_froggit_ext.h"
#include "bn_sprite_items_bt_froggit_head.h"
#include "bn_sprite_items_bt_froggit_legs.h"

namespace ut::game::bt::mob
{

namespace
{

enum ExtGfxIdx
{
    HURT = 0,
};

constexpr bn::fixed_point LEGS_DIFF = {0, 16};
constexpr bn::fixed_point HEAD_DIFF = {-1, -14.5f};
constexpr bn::fixed_point HURT_DIFF = {0, -3};

constexpr int LEGS_WAIT_UPDATE = 25 - 1;
constexpr int HEAD_WAIT_UPDATE = 50 - 1;

constexpr asset::Path<5> HEAD_PATH(bn::array<bn::fixed_point, 5>{
    bn::fixed_point(0, 0),
    bn::fixed_point(3, -0.5f),
    bn::fixed_point(6, 1),
    bn::fixed_point(3.5f, 2),
    bn::fixed_point(2.5f, -1.5f),
});

} // namespace

FroggitAnim::FroggitAnim(const bn::fixed_point& position)
    : _initPos(position), _legs(bn::sprite_items::bt_froggit_legs.create_sprite(position + LEGS_DIFF)),
      _head(bn::sprite_items::bt_froggit_head.create_sprite(position + HEAD_DIFF)),
      _hurt(bn::sprite_items::bt_froggit_ext.create_sprite(position + HURT_DIFF, ExtGfxIdx::HURT)),
      _legsAnim(bn::create_sprite_animate_action_forever(_legs, LEGS_WAIT_UPDATE,

                                                         bn::sprite_items::bt_froggit_legs.tiles_item(), 0, 1)),
      _headAnim(bn::create_sprite_animate_action_forever(_head, HEAD_WAIT_UPDATE,
                                                         bn::sprite_items::bt_froggit_head.tiles_item(), 0, 1)),
      _hurtShake(_hurt, action::ShakeStyle::ONE_WAY, {16, 0}, {2, 0}, 2),
      _headMove(_head, false, HEAD_PATH, bn::fixed(0.25f))
{
    _hurt.set_visible(false);
}

void FroggitAnim::render()
{
    switch (getAnimKind())
    {
    case MonsterAnimKind::STOP:
        break;

    case MonsterAnimKind::IDLE:
        _legsAnim.update();
        _headAnim.update();
        _headMove.update();
        break;

    case MonsterAnimKind::HURT:
        if (!_hurtShake.isDone())
        {
            _hurtShake.update();

            if (_hurtShake.isDone())
                start(MonsterAnimKind::STOP);
        }
        break;

    case MonsterAnimKind::KILLED:
        // TODO: animate vaporize
        break;

    default:
        BN_ERROR("Invalid animKind=", (int)getAnimKind());
    }
}

void FroggitAnim::startAnim(MonsterAnimKind animKind)
{
    // new: setup
    switch (animKind)
    {
    case MonsterAnimKind::IDLE:
        _legs.set_position(_initPos + LEGS_DIFF);
        _head.set_position(_initPos + HEAD_DIFF);
        _legsAnim.reset();
        _headAnim.reset();
        _legs.set_visible(true);
        _head.set_visible(true);
        _hurt.set_visible(false);
        break;

    case MonsterAnimKind::HURT:
        _legs.set_visible(false);
        _head.set_visible(false);
        _hurt.set_visible(true);
        _hurtShake.reset();
        break;

    case MonsterAnimKind::KILLED:
        // TODO: start vaporize
        _legs.set_visible(false);
        _head.set_visible(false);
        _hurt.set_visible(false);
        break;

    default:
        break;
    }
}

} // namespace ut::game::bt::mob
