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

bool ColliderPack::isTrigger() const
{
    return _isTrigger;
}

bool ColliderPack::isCollideWith(const ColliderPack& otherPack) const
{
    for (const auto& otherColl : otherPack._colls)
    {
        const auto& otherInfo = otherColl.getInfo();

        if (otherInfo.type == bn::type_id<coll::RectCollInfo>())
        {
            auto otherRect = otherInfo.rect;
            otherRect.position += otherPack._entity.getPosition();
            if (isCollideWith<coll::RectCollInfo>(otherRect))
                return true;
        }
        else if (otherInfo.type == bn::type_id<coll::CircleCollInfo>())
        {
            auto otherCircle = otherInfo.circle;
            otherCircle.position += otherPack._entity.getPosition();
            if (isCollideWith<coll::CircleCollInfo>(otherCircle))
                return true;
        }
        else if (otherInfo.type == bn::type_id<coll::AAIRTriCollInfo>())
        {
            auto otherTri = otherInfo.tri;
            otherTri.position += otherPack._entity.getPosition();
            if (isCollideWith<coll::AAIRTriCollInfo>(otherTri))
                return true;
        }
        else
            printCollInfoTypeError(otherInfo);
    }

    return false;
}

bool ColliderPack::isCollideWith(const coll::CollInfo& otherInfo) const
{
    if (otherInfo.type == bn::type_id<coll::RectCollInfo>())
        return isCollideWith(otherInfo.rect);
    else if (otherInfo.type == bn::type_id<coll::CircleCollInfo>())
        return isCollideWith(otherInfo.circle);
    else
        printCollInfoTypeError(otherInfo);

    return false;
}

} // namespace ut::game::cpnt
