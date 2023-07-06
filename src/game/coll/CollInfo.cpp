#include "game/coll/CollInfo.hpp"

#include <bn_fixed_rect.h>

namespace ut::game::coll
{

CollInfo::CollInfo(const bn::fixed_point& position_) : position(position_)
{
}

RectCollInfo::RectCollInfo(const bn::fixed_point& position_, const bn::fixed_size size_)
    : CollInfo(position_), size(size_)
{
}

CircleCollInfo::CircleCollInfo(const bn::fixed_point& position_, const bn::fixed radius_)
    : CollInfo(position_), radius(radius_)
{
}

auto RectCollInfo::getType() const -> bn::type_id_t
{
    return bn::type_id<RectCollInfo>();
}

auto CircleCollInfo::getType() const -> bn::type_id_t
{
    return bn::type_id<CircleCollInfo>();
}

static bool detectCollision(const RectCollInfo&, const RectCollInfo&);
static bool detectCollision(const RectCollInfo&, const CircleCollInfo&);
static bool detectCollision(const CircleCollInfo&, const RectCollInfo&);
static bool detectCollision(const CircleCollInfo&, const CircleCollInfo&);

/**
 * @brief Call `detectCollision(c1, c2)` of right type, if type matches.
 *
 * @param result if `detectCollision(c1, c2)` was called, its return value is stored in `result`.
 * @return whether or not `detectCollision(c1, c2)` was called
 */
template <typename CInfo1, typename CInfo2>
    requires(std::is_base_of_v<CollInfo, CInfo1> && std::is_base_of_v<CollInfo, CInfo2>)
static bool detectCollCaller(const CollInfo& c1, const CollInfo& c2, bool& result)
{
    if (c1.getType() == bn::type_id<CInfo1>() && c2.getType() == bn::type_id<CInfo2>())
    {
        result = detectCollision(static_cast<const CInfo1&>(c1), static_cast<const CInfo2&>(c2));
        return true;
    }

    return false;
}

bool CollInfo::isCollideWith(const CollInfo& other) const
{
    bool result = false;

    if (!detectCollCaller<RectCollInfo, RectCollInfo>(*this, other, result) &&
        !detectCollCaller<RectCollInfo, CircleCollInfo>(*this, other, result) &&
        !detectCollCaller<CircleCollInfo, RectCollInfo>(*this, other, result) &&
        !detectCollCaller<CircleCollInfo, CircleCollInfo>(*this, other, result))
        BN_ERROR("Invalid CollInfo type (this=", getType().internal_id(), ", other=", other.getType().internal_id(),
                 ")");

    return result;
}

static bn::fixed distanceSquared(const bn::fixed_point p1, const bn::fixed_point p2)
{
    const auto xDist = p1.x() - p2.x();
    const auto yDist = p1.y() - p2.y();

    return xDist * xDist + yDist * yDist;
}

static bool detectCollision(const RectCollInfo& c1, const RectCollInfo& c2)
{
    BN_ASSERT(c1.getType() == bn::type_id<RectCollInfo>());
    BN_ASSERT(c2.getType() == bn::type_id<RectCollInfo>());

    const bn::fixed_rect coll1(c1.position, c1.size);
    const bn::fixed_rect coll2(c2.position, c2.size);

    return coll1.intersects(coll2);
}

static bool detectCollision(const RectCollInfo& rectColl, const CircleCollInfo& circle)
{
    BN_ASSERT(rectColl.getType() == bn::type_id<RectCollInfo>());
    BN_ASSERT(circle.getType() == bn::type_id<CircleCollInfo>());

    const bn::fixed_rect rect{rectColl.position, rectColl.size};

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

static bool detectCollision(const CircleCollInfo& c1, const RectCollInfo& c2)
{
    BN_ASSERT(c1.getType() == bn::type_id<CircleCollInfo>());
    BN_ASSERT(c2.getType() == bn::type_id<RectCollInfo>());

    return detectCollision(c2, c1);
}

static bool detectCollision(const CircleCollInfo& c1, const CircleCollInfo& c2)
{
    BN_ASSERT(c1.getType() == bn::type_id<CircleCollInfo>());
    BN_ASSERT(c2.getType() == bn::type_id<CircleCollInfo>());

    const auto distSq = distanceSquared(c1.position, c2.position);
    const auto radSumSq = (c1.radius + c2.radius) * (c1.radius + c2.radius);

    return distSq < radSumSq;
}

} // namespace ut::game::coll
