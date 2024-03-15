#include "core/MovingBgBox.hpp"

namespace ut::core
{

MovingBgBox::MovingBgBox(const bn::top_left_fixed_rect& boxRect) : _bgBox(boxRect), _dest(boxRect)
{
}

void MovingBgBox::update()
{
    if (!isDoneMoving())
    {
        const auto prevBoxRect = _bgBox.getRect();

        auto x1 = prevBoxRect.left();
        auto y1 = prevBoxRect.top();
        auto x2 = prevBoxRect.right();
        auto y2 = prevBoxRect.bottom();

        if (x1 > _dest.x())
            x1 = bn::max(x1 - SPEED, _dest.x());
        else if (x1 < _dest.x())
            x1 = bn::min(x1 + SPEED, _dest.x());
        if (y1 > _dest.y())
            y1 = bn::max(y1 - SPEED, _dest.y());
        else if (y1 < _dest.y())
            y1 = bn::min(y1 + SPEED, _dest.y());

        if (x2 > _dest.x() + _dest.width())
            x2 = bn::max(x2 - SPEED, _dest.x() + _dest.width());
        else if (x2 < _dest.x() + _dest.width())
            x2 = bn::min(x2 + SPEED, _dest.x() + _dest.width());
        if (y2 > _dest.y() + _dest.height())
            y2 = bn::max(y2 - SPEED, _dest.y() + _dest.height());
        else if (y2 < _dest.y() + _dest.height())
            y2 = bn::min(y2 + SPEED, _dest.y() + _dest.height());

        _bgBox.setRect(bn::top_left_fixed_rect(x1, y1, x2 - x1, y2 - y1));
    }
}

bool MovingBgBox::isDoneMoving() const
{
    return _bgBox.getRect() == _dest;
}

void MovingBgBox::moveTo(const bn::top_left_fixed_rect& boxRect)
{
    _dest = boxRect;
}

} // namespace ut::core
