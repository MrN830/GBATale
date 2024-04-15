#include "game/bt/mob/WhimsunAnim.hpp"

#include "bn_sprite_items_bt_whimsun_main.h"

namespace ut::game::bt::mob
{

namespace
{

enum GfxIdx
{
    HURT = 2
};

} // namespace

WhimsunAnim::WhimsunAnim(const bn::fixed_point& position)
    : _spr(bn::sprite_items::bt_whimsun_main.create_sprite(position)),
      _sprAnim(decltype(_sprAnim)::forever(_spr, 10, bn::sprite_items::bt_whimsun_main.tiles_item(),
                                           bn::array<uint16_t, 2>{0, 1})),
      _sprMoveUpDown(_spr, 32, position + bn::fixed_point(0, -8)),
      _sprShake(_spr, action::ShakeStyle::ONE_WAY, {16, 0}, {2, 0}, 2)
{
}

void WhimsunAnim::render()
{
    switch (getAnimKind())
    {
    case MonsterAnimKind::STOP:
        break;

    case MonsterAnimKind::IDLE:
        _sprAnim.update();
        _sprMoveUpDown.update();
        break;

    case MonsterAnimKind::HURT:
        if (!_sprShake.isDone())
        {
            _sprShake.update();

            if (_sprShake.isDone())
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

void WhimsunAnim::startAnim(MonsterAnimKind animKind)
{
    // prev: clean-up
    switch (getAnimKind())
    {
    case MonsterAnimKind::IDLE:
        _sprAnim.reset();
        // reset to init position if prev anim was `IDLE`
        _sprMoveUpDown.reset();
        break;

    default:
        break;
    }

    // new: setup
    switch (animKind)
    {
    case MonsterAnimKind::HURT:
        _spr.set_tiles(bn::sprite_items::bt_whimsun_main.tiles_item(), GfxIdx::HURT);
        _sprShake.reset();
        break;

    case MonsterAnimKind::KILLED:
        // TODO: start vaporize
        _spr.set_visible(false);
        break;

    default:
        break;
    }
}

} // namespace ut::game::bt::mob
