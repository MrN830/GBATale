#include "core/Directions.hpp"

#include <bn_math.h>

namespace ut::core
{

auto dirs2Pos(Directions dirs) -> bn::fixed_point
{
    bn::point result{0, 0};

    if (!!(dirs & Directions::UP))
        result.set_y(result.y() - 1);
    if (!!(dirs & Directions::DOWN))
        result.set_y(result.y() + 1);
    if (!!(dirs & Directions::LEFT))
        result.set_x(result.x() - 1);
    if (!!(dirs & Directions::RIGHT))
        result.set_x(result.x() + 1);

    return result;
}

auto pos2Dirs(const bn::fixed_point& pos, bool strictDiagonal) -> Directions
{
    auto result = Directions::NONE;

    bool useX = true, useY = true;

    if (strictDiagonal)
    {
        const auto absX = bn::abs(pos.x());
        const auto absY = bn::abs(pos.y());

        if (absX > absY)
            useY = false;
        else if (absX < absY)
            useX = false;
    }

    if (useY)
    {
        if (pos.y() < 0)
            result |= Directions::UP;
        else if (pos.y() > 0)
            result |= Directions::DOWN;
    }
    if (useX)
    {
        if (pos.x() < 0)
            result |= Directions::LEFT;
        else if (pos.x() > 0)
            result |= Directions::RIGHT;
    }

    return result;
}

} // namespace ut::core
