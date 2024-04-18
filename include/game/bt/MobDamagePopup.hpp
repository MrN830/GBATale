#pragma once

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "game/bt/HpBar.hpp"

namespace ut::core
{
class TextGens;
}

namespace ut::game::bt
{

class MobDamagePopup
{
public:
    MobDamagePopup(core::TextGens&);

    void render();

    void start(const bn::fixed_point& position, int hpBarWidth, int rawDamage, int prevHp, int curHp, int maxHp);

    void removeWithDelay(int delay);

private:
    bool isShowing() const;

private:
    core::TextGens& _textGens;

    HpBar _hpBar;
    bn::vector<bn::sprite_ptr, 5> _textSprs;

    int _removeCountdown = 0;

    bn::fixed_point _startPos;

    // hp bar related
    int _hpBarWidth = 0;
    int _rawDamage = 0;
    bn::fixed _apparentHp = 0; // currently rendered
    bn::fixed _destHp = 0;
    bn::fixed _maxHp = 0;
    bool _shouldRenderDecrease = false;

    // damage number related
    bn::fixed _ySpeed;
    bool _isMiss = false;
    bool _isBounceDone = true;
};

} // namespace ut::game::bt
