#pragma once

#include "core/BgBox.hpp"

namespace ut::core
{

class MovingBgBox
{
private:
    static constexpr bn::fixed SPEED = 6;

public:
    MovingBgBox(const bn::top_left_fixed_rect& boxRect, int bgPriority = 3);

    void update();

public:
    bool isDoneMoving() const;

    void moveTo(const bn::top_left_fixed_rect& boxRect);

private:
    BgBox _bgBox;
    bn::top_left_fixed_rect _dest;
};

} // namespace ut::core
