#pragma once

#include <bn_fixed_point.h>
#include <bn_fixed_size.h>

namespace ut::core
{

enum class ShakeStyle : uint8_t
{
    ONE_WAY, // even frame shakes move value to `initPos`
    TWO_WAY, // even frame shakes move value to `initPos - shakeScale`, resulting in `initPos` being the center
};

/**
 * @brief Butano-style position shaker.
 *
 * Unlike Butano's value_template_actions, `Value` is referenced, so it should outlive the `ShakeValueTemplateAction` to
 * avoid dangling reference.
 *
 * @tparam Value Value to modify.
 * @tparam PositionManager Reads and writes the position of the value to modify.
 */
template <typename Value, typename PositionManager>
class ShakeValueTemplateAction
{
public:
    void reset()
    {
        _curScale = _initScale;
        _countdown = 1;
        _isShakeDirectionPositive = true;

        PositionManager::set(_initPos, _value);
    }

    void update()
    {
        if (isDone())
            return;

        if (--_countdown == 0)
        {
            // move to new position
            auto pos = _initPos;

            if (_style == ShakeStyle::ONE_WAY)
            {
                if (_isShakeDirectionPositive)
                    pos += _curScale;
            }
            else if (_style == ShakeStyle::TWO_WAY)
            {
                if (_isShakeDirectionPositive)
                    pos += _curScale;
                else
                    pos -= _curScale;
            }
            else
                BN_ERROR("Invalid shake style=", (int)_style);

            PositionManager::set(pos, _value);

            // update shake infos
            _countdown = _durationUpdates;
            _isShakeDirectionPositive = !_isShakeDirectionPositive;

            if (_isShakeDirectionPositive)
            {
                _curScale.set_x(bn::max(bn::fixed(0), _curScale.x() - _decrease.width()));
                _curScale.set_y(bn::max(bn::fixed(0), _curScale.y() - _decrease.height()));

                if (_curScale == bn::fixed_point(0, 0))
                    _countdown = -1;
            }
        }
    }

    bool isDone() const
    {
        return _countdown < 0;
    }

protected:
    ShakeValueTemplateAction(Value& value, ShakeStyle shakeStyle, const bn::fixed_size& shakeScale,
                             const bn::fixed_size& shakeDecrease, int durationUpdates)
        : _value(value), _durationUpdates(durationUpdates), _initPos(PositionManager::get(_value)),
          _initScale({shakeScale.width(), shakeScale.height()}),
          _decrease({shakeDecrease.width(), shakeDecrease.height()}), _curScale(_initScale), _style(shakeStyle)
    {
        BN_ASSERT(durationUpdates > 0, "Invalid durationUpdates=", durationUpdates);

        BN_ASSERT(shakeScale != bn::fixed_size(0, 0), "Invalid shakeScale={", shakeScale.width(), ", ",
                  shakeScale.height(), "}");
        BN_ASSERT(shakeDecrease != bn::fixed_size(0, 0), "Invalid shakeDecrease={", shakeDecrease.width(), ", ",
                  shakeDecrease.height(), "}");
    }

private:
    Value& _value;

    const int _durationUpdates;
    const bn::fixed_point _initPos;
    const bn::fixed_point _initScale;
    const bn::fixed_size _decrease;

    bn::fixed_point _curScale;
    int _countdown = 1; // `-1` if done

    const ShakeStyle _style;

    bool _isShakeDirectionPositive = true;
};

} // namespace ut::core
