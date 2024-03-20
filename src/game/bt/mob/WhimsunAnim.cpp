#include "game/bt/mob/WhimsunAnim.hpp"

#include "bn_sprite_items_bt_whimsun_main.h"

namespace ut::game::bt::mob
{

WhimsunAnim::WhimsunAnim(const bn::fixed_point& position)
    : _spr(bn::sprite_items::bt_whimsun_main.create_sprite(position)),
      _sprAnim(decltype(_sprAnim)::forever(_spr, 10, bn::sprite_items::bt_whimsun_main.tiles_item(),
                                           bn::array<uint16_t, 2>{0, 1})),
      _sprMoveUpDown(_spr, 32, position + bn::fixed_point(0, -8))
{
}

void WhimsunAnim::render()
{
    if (getAnimKind() == MonsterAnimKind::IDLE)
    {
        _sprAnim.update();
        _sprMoveUpDown.update();
    }
}

void WhimsunAnim::startAnim(MonsterAnimKind)
{
    // TODO
}

} // namespace ut::game::bt::mob
