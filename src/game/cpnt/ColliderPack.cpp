#include "game/cpnt/ColliderPack.hpp"

namespace ut::game::cpnt
{

static void printCollInfoTypeError(const coll::CollInfo& coll)
{
    BN_ERROR("Invalid CollInfo type=", coll.type.internal_id());
}

ColliderPack::ColliderPack(ent::Entity& entity, bool isTrigger) : Component(entity), _isTrigger(isTrigger)
{
}

auto ColliderPack::getType() const -> bn::type_id_t
{
    return bn::type_id<ColliderPack>();
}

void ColliderPack::setStaticCollInfos(const bn::span<const coll::RectCollInfo>& staticColls)
{
    _staticColls = staticColls;
}

bool ColliderPack::isTrigger() const
{
    return _isTrigger;
}

bool ColliderPack::isCollideWith(const ColliderPack& otherPack) const
{
    auto checkRectCollision = [this](const coll::RectCollInfo& otherRectInfo, const bn::fixed_point& otherPos) -> bool {
        auto otherRect = otherRectInfo;
        otherRect.position += otherPos;
        return isCollideWith<coll::RectCollInfo>(otherRect);
    };

    auto checkCollision = [this, &checkRectCollision](const coll::CollInfo& otherInfo,
                                                      const bn::fixed_point& otherPos) -> bool {
        if (otherInfo.type == bn::type_id<coll::RectCollInfo>())
        {
            if (checkRectCollision(otherInfo.rect, otherPos))
                return true;
        }
        else if (otherInfo.type == bn::type_id<coll::CircleCollInfo>())
        {
            auto otherCircle = otherInfo.circle;
            otherCircle.position += otherPos;
            if (isCollideWith<coll::CircleCollInfo>(otherCircle))
                return true;
        }
        else if (otherInfo.type == bn::type_id<coll::AAIRTriCollInfo>())
        {
            auto otherTri = otherInfo.tri;
            otherTri.position += otherPos;
            if (isCollideWith<coll::AAIRTriCollInfo>(otherTri))
                return true;
        }
        else
            printCollInfoTypeError(otherInfo);

        return false;
    };

    const auto& otherPos = otherPack._entity.getPosition();

    for (const auto& otherRectInfo : otherPack._staticColls)
    {
        if (checkRectCollision(otherRectInfo, otherPos))
            return true;
    }
    for (const auto& otherColl : otherPack._dynamicColls)
    {
        const auto& otherInfo = otherColl.getInfo();
        if (checkCollision(otherInfo, otherPos))
            return true;
    }

    return false;
}

bool ColliderPack::isCollideWith(const coll::CollInfo& otherInfo) const
{
    if (otherInfo.type == bn::type_id<coll::RectCollInfo>())
        return isCollideWith(otherInfo.rect);
    else if (otherInfo.type == bn::type_id<coll::CircleCollInfo>())
        return isCollideWith(otherInfo.circle);
    else if (otherInfo.type == bn::type_id<coll::AAIRTriCollInfo>())
        return isCollideWith(otherInfo.tri);
    else
        printCollInfoTypeError(otherInfo);

    return false;
}

} // namespace ut::game::cpnt
