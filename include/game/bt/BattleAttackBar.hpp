#pragma once

#include <bn_sprite_animate_actions.h>
#include <bn_sprite_ptr.h>

#include <bn_value_template_actions.h>

namespace ut::game::bt
{

class BattleAttackBar
{
public:
    BattleAttackBar(const bn::fixed_point& position);

    void render();

public:
    bool isVisible() const;
    void setVisible(bool isVisible);

    auto getPosition() const -> const bn::fixed_point&;
    void setPosition(const bn::fixed_point& position);

    void startFlicker();

private:
    bn::sprite_ptr _spr;
    bn::sprite_animate_action<2> _sprFlicker;

    bool _isFlickerStarted = false;
};

class BattleAttackBarPositionManager
{
public:
    static auto get(const BattleAttackBar& bar) -> const bn::fixed_point&
    {
        return bar.getPosition();
    }

    static void set(const bn::fixed_point& pos, BattleAttackBar& bar)
    {
        bar.setPosition(pos);
    }
};

class BattleAttackBarMoveByAction
    : public bn::by_value_template_action<BattleAttackBar, bn::fixed_point, BattleAttackBarPositionManager>
{
public:
    BattleAttackBarMoveByAction(BattleAttackBar& bar, const bn::fixed_point& deltaPos)
        : by_value_template_action(bar, deltaPos)
    {
    }
};

} // namespace ut::game::bt
