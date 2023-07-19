#include "game/coll/CollInfo.hpp"

#include <bn_fixed_rect.h>
#include <bn_math.h>

namespace ut::game::coll
{

static void printTypeError(const CollInfo& coll)
{
    BN_ERROR("Invalid CollInfo type=", (void*)coll.type.internal_id());
}

static bn::fixed distanceSquared(const bn::fixed_point p1, const bn::fixed_point p2)
{
    const auto xDist = p1.x() - p2.x();
    const auto yDist = p1.y() - p2.y();

    return xDist * xDist + yDist * yDist;
}

bool RectCollInfo::isCollideWith(const CollInfo& other) const
{
    return other.isCollideWith(*this);
}

bool RectCollInfo::isCollideWith(const RectCollInfo& otherRect) const
{
    const bn::fixed_rect r1(this->position, this->size);
    const bn::fixed_rect r2(otherRect.position, otherRect.size);

    return r1.intersects(r2);
}

bool RectCollInfo::isCollideWith(const CircleCollInfo& circle) const
{
    const bn::fixed_rect rect(this->position, this->size);

    bn::fixed_point rectClosestPoint{
        circle.position.x() < rect.left()    ? rect.left()
        : circle.position.x() > rect.right() ? rect.right()
                                             : circle.position.x(),
        circle.position.y() < rect.top()      ? rect.top()
        : circle.position.y() > rect.bottom() ? rect.bottom()
                                              : circle.position.y(),
    };

    const auto distSq = distanceSquared(rectClosestPoint, circle.position);
    const auto radSq = circle.radius * circle.radius;

    return distSq < radSq;
}

static constexpr auto triPlus(const bn::fixed_point& p) -> bn::fixed
{
    return p.x() + p.y();
};

static constexpr auto triMinus(const bn::fixed_point& p) -> bn::fixed
{
    return p.x() - p.y();
};

bool RectCollInfo::isCollideWith(const AAIRTriCollInfo& tri) const
{
    using Dirs = core::Directions;

    const bool upLeft = (!!(tri.directions & Dirs::UP) && !!(tri.directions & Dirs::LEFT));
    const bool upRight = (!!(tri.directions & Dirs::UP) && !!(tri.directions & Dirs::RIGHT));
    const bool downLeft = (!!(tri.directions & Dirs::DOWN) && !!(tri.directions & Dirs::LEFT));
    const bool downRight = (!!(tri.directions & Dirs::DOWN) && !!(tri.directions & Dirs::RIGHT));

    const bn::fixed_rect rect(this->position, this->size);
    const bn::fixed_rect triRect{
        tri.position.x() + (tri.legLength / 2) * ((upLeft || downLeft) ? +1 : -1),
        tri.position.y() + (tri.legLength / 2) * ((upLeft || upRight) ? +1 : -1),
        tri.legLength,
        tri.legLength,
    };

    // x-axis, y-axis AABB
    if (!rect.intersects(triRect))
        return false;

    // tri-axis AABB
    const auto triAxis = ((upLeft || downRight) ? triPlus : triMinus);

    const auto triLow =
        ((upLeft || upRight) ? bn::min(triAxis(triRect.top_left()), triAxis(triRect.top_right()))
                             : bn::min(triAxis(triRect.bottom_left()), triAxis(triRect.bottom_right())));
    const auto triHigh =
        ((upLeft || upRight) ? bn::max(triAxis(triRect.top_left()), triAxis(triRect.top_right()))
                             : bn::max(triAxis(triRect.bottom_left()), triAxis(triRect.bottom_right())));
    const auto rectLow = ((triAxis == triPlus) ? bn::min(triAxis(rect.top_left()), triAxis(rect.bottom_right()))
                                               : bn::min(triAxis(rect.top_right()), triAxis(rect.bottom_left())));
    const auto rectHigh = ((triAxis == triPlus) ? bn::max(triAxis(rect.top_left()), triAxis(rect.bottom_right()))
                                                : bn::max(triAxis(rect.top_right()), triAxis(rect.bottom_left())));

    return rectLow < triHigh && rectHigh > triLow;
}

bool CircleCollInfo::isCollideWith(const CollInfo& other) const
{
    return other.isCollideWith(*this);
}

bool CircleCollInfo::isCollideWith(const RectCollInfo& otherRect) const
{
    return otherRect.isCollideWith(*this);
}

bool CircleCollInfo::isCollideWith(const CircleCollInfo& otherCircle) const
{
    const auto distSq = distanceSquared(this->position, otherCircle.position);
    const auto radSumSq = (this->radius + otherCircle.radius) * (this->radius + otherCircle.radius);

    return distSq < radSumSq;
}

bool CircleCollInfo::isCollideWith(const AAIRTriCollInfo&) const
{
    BN_ERROR("Circle <-> AAIRTri collision not supported");
    return false;
}

bool AAIRTriCollInfo::isCollideWith(const CollInfo& other) const
{
    return other.isCollideWith(*this);
}

bool AAIRTriCollInfo::isCollideWith(const RectCollInfo& otherRect) const
{
    return otherRect.isCollideWith(*this);
}

bool AAIRTriCollInfo::isCollideWith(const CircleCollInfo& otherCircle) const
{
    return otherCircle.isCollideWith(*this);
}

bool AAIRTriCollInfo::isCollideWith(const AAIRTriCollInfo&) const
{
    BN_ERROR("AAIRTri <-> AAIRTri collision not supported");
    return false;
}

bool CollInfo::isCollideWith(const CollInfo& other) const
{
    if (other.type == bn::type_id<RectCollInfo>())
        return isCollideWith(other.rect);
    else if (other.type == bn::type_id<CircleCollInfo>())
        return isCollideWith(other.circle);
    else if (other.type == bn::type_id<AAIRTriCollInfo>())
        return isCollideWith(other.tri);
    else
        printTypeError(other);

    return false;
}

bool CollInfo::isCollideWith(const RectCollInfo& otherRect) const
{
    if (this->type == bn::type_id<RectCollInfo>())
        return rect.isCollideWith(otherRect);
    else if (this->type == bn::type_id<CircleCollInfo>())
        return circle.isCollideWith(otherRect);
    else if (this->type == bn::type_id<AAIRTriCollInfo>())
        return tri.isCollideWith(otherRect);
    else
        printTypeError(*this);

    return false;
}

bool CollInfo::isCollideWith(const CircleCollInfo& otherCircle) const
{
    if (this->type == bn::type_id<RectCollInfo>())
        return rect.isCollideWith(otherCircle);
    else if (this->type == bn::type_id<CircleCollInfo>())
        return circle.isCollideWith(otherCircle);
    else if (this->type == bn::type_id<AAIRTriCollInfo>())
        return tri.isCollideWith(otherCircle);
    else
        printTypeError(*this);

    return false;
}

bool CollInfo::isCollideWith(const AAIRTriCollInfo& otherTri) const
{
    if (this->type == bn::type_id<RectCollInfo>())
        return rect.isCollideWith(otherTri);
    else if (this->type == bn::type_id<CircleCollInfo>())
        return circle.isCollideWith(otherTri);
    else if (this->type == bn::type_id<AAIRTriCollInfo>())
        return tri.isCollideWith(otherTri);
    else
        printTypeError(*this);

    return false;
}

} // namespace ut::game::coll
