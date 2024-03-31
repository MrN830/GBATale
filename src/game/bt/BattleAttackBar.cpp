#include "game/bt/BattleAttackBar.hpp"

#include "game/bt/bt_consts.hpp"

#include "bn_sprite_items_bt_attack_bar.h"

namespace ut::game::bt
{

namespace
{

constexpr int BAR_FLICKER_WAIT_UPDATE = 2;

}

BattleAttackBar::BattleAttackBar(const bn::fixed_point& position)
    : _spr(bn::sprite_items::bt_attack_bar.create_sprite(position, 0)),
      _sprFlicker(bn::create_sprite_animate_action_forever(_spr, BAR_FLICKER_WAIT_UPDATE,
                                                           bn::sprite_items::bt_attack_bar.tiles_item(), 0, 1))
{
    _spr.set_visible(false);
    _spr.set_bg_priority(consts::BG_ATK_PRIORITY);
}

void BattleAttackBar::render()
{
    if (_isFlickerStarted && !_sprFlicker.done())
        _sprFlicker.update();
}

bool BattleAttackBar::isVisible() const
{
    return _spr.visible();
}

void BattleAttackBar::setVisible(bool isVisible)
{
    _spr.set_visible(isVisible);
}

auto BattleAttackBar::getPosition() const -> const bn::fixed_point&
{
    return _spr.position();
}

void BattleAttackBar::setPosition(const bn::fixed_point& position)
{
    _spr.set_position(position);
}

void BattleAttackBar::startFlicker()
{
    _isFlickerStarted = true;
}

} // namespace ut::game::bt
