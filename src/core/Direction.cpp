#include "core/Directions.hpp"

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

auto pos2Dirs(const bn::fixed_point& pos) -> Directions
{
    auto result = Directions::NONE;

    if (pos.y() < 0)
        result |= Directions::UP;
    else if (pos.y() > 0)
        result |= Directions::DOWN;

    if (pos.x() < 0)
        result |= Directions::LEFT;
    else if (pos.x() > 0)
        result |= Directions::RIGHT;

    return result;
}

} // namespace ut::core
