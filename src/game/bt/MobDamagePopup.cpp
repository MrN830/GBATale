#include "game/bt/MobDamagePopup.hpp"

#include <bn_string.h>

#include "core/TextGens.hpp"

#include "game/bt/bt_consts.hpp"

namespace ut::game::bt
{

namespace
{

constexpr int HP_BAR_HEIGHT = 6;

constexpr bn::color DARK_GRAY(8, 8, 8);

constexpr bn::fixed_point TEXT_DIFF = {0, -13};
constexpr bn::fixed_point MISS_DIFF = {0, +6};
constexpr bn::fixed_point HP_BAR_DIFF = {0, 0};

constexpr bn::fixed START_Y_SPEED = -2;
constexpr bn::fixed GRAVITY = 0.25f;

} // namespace

MobDamagePopup::MobDamagePopup(core::TextGens& textGens)
    : _textGens(textGens),
      _hpBar(consts::BG_ATK_PRIORITY, true, {0, 0}, HP_BAR_HEIGHT, 51, 51, bn::colors::lime, DARK_GRAY)
{
    _hpBar.setVisible(false);
}

void MobDamagePopup::render()
{
    if (!isShowing())
        return;

    if (!_isMiss)
    {
        // decrease hp bar
        if (_shouldRenderDecrease)
        {
            if (_apparentHp > _destHp)
            {
                _apparentHp -= _rawDamage / bn::fixed(15);
                if (_apparentHp < _destHp)
                    _apparentHp = _destHp;

                _hpBar.setHealthWidth((_hpBarWidth * _apparentHp / _maxHp).round_integer());
            }
        }

        _shouldRenderDecrease = !_shouldRenderDecrease; // even frames only

        if (!_isBounceDone)
        {
            // render damage number bounce
            _ySpeed += GRAVITY;
            for (auto& spr : _textSprs)
                spr.set_y(spr.y() + _ySpeed);

            // stop bounce
            if (_ySpeed > -START_Y_SPEED)
            {
                for (auto& spr : _textSprs)
                    spr.set_y(_startPos.y() + TEXT_DIFF.y());

                _ySpeed = 0;
                _isBounceDone = true;
            }
        }
    }

    // remove on timeout
    if (_removeCountdown > 0)
    {
        if (--_removeCountdown == 0)
        {
            _hpBar.setVisible(false);
            _textSprs.clear();
        }
    }
}

void MobDamagePopup::start(const bn::fixed_point& position, int hpBarWidth, int rawDamage, int prevHp, int curHp,
                           int maxHp)
{
    _isMiss = (rawDamage <= 0);

    // start hp bar decrease
    if (!_isMiss)
    {
        BN_ASSERT(maxHp > 0);
        _hpBar.setWidth(hpBarWidth, hpBarWidth * prevHp / maxHp);
        _hpBar.setTopLeftPos(position - bn::fixed_point(hpBarWidth, _hpBar.getHeight()) / 2 + HP_BAR_DIFF);
        _hpBar.setVisible(true);

        _hpBarWidth = hpBarWidth;
        _rawDamage = rawDamage;
        _apparentHp = prevHp;
        _destHp = curHp;
        _maxHp = maxHp;
        _shouldRenderDecrease = false;
    }

    // generate damage number
    _textSprs.clear();
    auto& textGen = _textGens.get(asset::FontKind::DMG_NUMS);
    const auto damageText = _isMiss ? "MISS" : bn::to_string<10>(rawDamage);

    const auto prevAlign = textGen.alignment();
    textGen.set_center_alignment();
    textGen.generate(position + TEXT_DIFF + (_isMiss ? MISS_DIFF : bn::fixed_point{0, 0}), damageText, _textSprs);
    textGen.set_alignment(prevAlign);

    for (auto& spr : _textSprs)
        spr.set_bg_priority(consts::BG_ATK_PRIORITY);

    // start damage number bounce
    _startPos = position;
    _ySpeed = START_Y_SPEED;

    _isBounceDone = false;
}

void MobDamagePopup::removeWithDelay(int delay)
{
    BN_ASSERT(delay >= 0, "negative delay=", delay);

    if (!isShowing())
        return;

    _removeCountdown = delay + 1;
}

bool MobDamagePopup::isShowing() const
{
    return !_textSprs.empty() && _textSprs[0].visible();
}

} // namespace ut::game::bt
