#include <bn_fixed_point.h>

#include "game/ent/Entity.hpp"

namespace ut::game::cpnt
{

template <typename TColl>
    requires std::is_base_of_v<coll::Collider, TColl>
void ColliderPack::addCollider(TColl& coll)
{
    _colls.push_front(coll);
}

template <typename CInfo>
bool ColliderPack::isCollideWith(const CInfo& other) const
{
    for (const auto& coll : _colls)
    {
        const auto& info = coll.getInfo();

        if (info.type == bn::type_id<coll::RectCollInfo>())
        {
            auto rect = info.rect;
            rect.position += _entity.getPosition();
            if (rect.isCollideWith(other))
                return true;
        }
        else if (info.type == bn::type_id<coll::CircleCollInfo>())
        {
            auto circle = info.circle;
            circle.position += _entity.getPosition();
            if (circle.isCollideWith(other))
                return true;
        }
        else if (info.type == bn::type_id<coll::AAIRTriCollInfo>())
        {
            auto tri = info.tri;
            tri.position += _entity.getPosition();
            if (tri.isCollideWith(other))
                return true;
        }
        else
            BN_ERROR("Invalid CollInfo type=", info.type.internal_id());
    }

    return false;
}

} // namespace ut::game::cpnt
