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
      _moveAction(_spr, DURATION_UPDATES, position + DIFF_POS)
{
}

void MoldsmalAnim::render()
{
    if (getAnimKind() == MonsterAnimKind::IDLE)
    {
        _scaleAction.update();
        _moveAction.update();
    }
}

void MoldsmalAnim::startAnim(MonsterAnimKind)
{
    // TODO
}

} // namespace ut::game::bt::mob
