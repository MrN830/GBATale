#include "game/cpnt/ColliderPack.hpp"

#include "game/ent/Entity.hpp"

namespace ut::game::cpnt
{

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
        const auto& otherRelInfo = otherColl.getInfo();

        if (otherRelInfo.getType() == bn::type_id<coll::RectCollInfo>())
            return isCollideWithOtherRel(otherPack._entity.getPosition(),
                                         static_cast<const coll::RectCollInfo&>(otherRelInfo));
        else if (otherRelInfo.getType() == bn::type_id<coll::CircleCollInfo>())
            return isCollideWithOtherRel(otherPack._entity.getPosition(),
                                         static_cast<const coll::CircleCollInfo&>(otherRelInfo));
        else
            BN_ERROR("Invalid other coll info type=", otherRelInfo.getType().internal_id());
    }

    return false;
}

bool ColliderPack::isCollideWith(const coll::CollInfo& otherInfo) const
{
    for (const auto& coll : _colls)
    {
        const auto& relInfo = coll.getInfo();

        if (relInfo.getType() == bn::type_id<coll::RectCollInfo>())
            isCollideWithThisRel(_entity.getPosition(), static_cast<const coll::RectCollInfo&>(relInfo), otherInfo);
        else if (relInfo.getType() == bn::type_id<coll::CircleCollInfo>())
            isCollideWithThisRel(_entity.getPosition(), static_cast<const coll::CircleCollInfo&>(relInfo), otherInfo);
        else
            BN_ERROR("Invalid coll info type=", relInfo.getType().internal_id());
    }

    return false;
}

} // namespace ut::game::cpnt
