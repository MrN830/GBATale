#include "game/bt/mob/MoldsmalAnim.hpp"

#include "bn_sprite_items_bt_moldsmal_main.h"

namespace ut::game::bt::mob
{

namespace
{

constexpr int DURATION_UPDATES = 20;

constexpr bn::fixed MIN_SCALE = 1 - 0.1 * 64 / 42;
constexpr bn::fixed MAX_SCALE = 1 + 0.1 * 64 / 42;

constexpr bn::fixed_point DIFF_POS = {0, -10.9};

auto initSpr(const bn::fixed_point& position) -> bn::sprite_ptr
{
    auto spr = bn::sprite_items::bt_moldsmal_main.create_sprite(position);
    spr.set_scale(MIN_SCALE);
    return spr;
}

} // namespace

MoldsmalAnim::MoldsmalAnim(const bn::fixed_point& position)
    : _spr(initSpr(position)), _scaleAction(_spr, DURATION_UPDATES, MAX_SCALE),
      _moveAction(_spr, DURATION_UPDATES, position + DIFF_POS),
      _sprShake(_spr, core::ShakeStyle::ONE_WAY, {16, 0}, {2, 0}, 2)
{
}

void MoldsmalAnim::render()
{
    switch (getAnimKind())
    {
    case MonsterAnimKind::STOP:
        break;

    case MonsterAnimKind::IDLE:
        _scaleAction.update();
        _moveAction.update();
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

void MoldsmalAnim::startAnim(MonsterAnimKind animKind)
{
    // prev: clean-up
    switch (getAnimKind())
    {
    case MonsterAnimKind::IDLE:
        _scaleAction.reset();
        _moveAction.reset();
        break;

    default:
        break;
    }

    // new: setup
    switch (animKind)
    {
    case MonsterAnimKind::HURT:
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
