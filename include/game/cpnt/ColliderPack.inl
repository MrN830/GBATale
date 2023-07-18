#include <bn_fixed_point.h>

#include "game/ent/Entity.hpp"

namespace ut::game::cpnt
{

template <typename TColl>
    requires std::is_base_of_v<coll::Collider, TColl>
void ColliderPack::addDynamicCollider(TColl& coll)
{
    _dynamicColls.push_front(coll);
}

template <typename CInfo>
bool ColliderPack::isCollideWith(const CInfo& other) const
{
    auto checkRectCollision = [&other](const coll::RectCollInfo& rectInfo, const bn::fixed_point& pos) -> bool {
        auto rect = rectInfo;
        rect.position += pos;
        return rect.isCollideWith(other);
    };

    auto checkCollision = [&other, &checkRectCollision](const coll::CollInfo& info,
                                                        const bn::fixed_point& pos) -> bool {
        if (info.type == bn::type_id<coll::RectCollInfo>())
        {
            if (checkRectCollision(info.rect, pos))
                return true;
        }
        else if (info.type == bn::type_id<coll::CircleCollInfo>())
        {
            auto circle = info.circle;
            circle.position += pos;
            if (circle.isCollideWith(other))
                return true;
        }
        else if (info.type == bn::type_id<coll::AAIRTriCollInfo>())
        {
            auto tri = info.tri;
            tri.position += pos;
            if (tri.isCollideWith(other))
                return true;
        }
        else
            BN_ERROR("Invalid CollInfo type=", info.type.internal_id());

        return false;
    };

    const auto& pos = _entity.getPosition();

    for (const auto& rectInfo : _staticColls)
    {
        if (checkRectCollision(rectInfo, pos))
            return true;
    }
    for (const auto& coll : _dynamicColls)
    {
        const auto& info = coll.getInfo();
        if (checkCollision(info, pos))
            return true;
    }

    return false;
}

} // namespace ut::game::cpnt
