#pragma once

#include "game/cpnt/Component.hpp"

#include "game/coll/Collider.hpp"

namespace ut::game::sys
{
class EntityManager;
}

namespace ut::game::cpnt
{

class ColliderPack : public Component
{
    // `EntityManager` deletes colliders from the coll pool on entity delete
    friend class sys::EntityManager;

public:
    ColliderPack(ent::Entity&, bool isTrigger);

    auto getType() const -> bn::type_id_t override;

    template <typename TColl>
        requires std::is_base_of_v<coll::Collider, TColl>
    void addCollider(TColl& coll)
    {
        _colls.push_front(coll);
    }

    bool isTrigger() const;

    bool isCollideWith(const ColliderPack& other) const;
    bool isCollideWith(const coll::CollInfo& other) const;

private:
    template <typename CInfo>
        requires std::is_base_of_v<coll::CollInfo, CInfo>
    static auto getAbsCollInfo(const bn::fixed_point& entityPos, const CInfo& relInfo) -> CInfo
    {
        auto result = relInfo;
        result.position += entityPos;
        return result;
    }

    template <typename CInfo>
        requires std::is_base_of_v<coll::CollInfo, CInfo>
    bool isCollideWithOtherRel(const bn::fixed_point& entityPos, const CInfo& otherRelInfo) const
    {
        const auto otherAbsInfo = getAbsCollInfo(entityPos, otherRelInfo);
        return isCollideWith(otherAbsInfo);
    }

    template <typename CInfo>
        requires std::is_base_of_v<coll::CollInfo, CInfo>
    bool isCollideWithThisRel(const bn::fixed_point& entityPos, const CInfo& relInfo,
                              const coll::CollInfo& otherInfo) const
    {
        const auto absInfo = getAbsCollInfo(entityPos, relInfo);
        return absInfo.isCollideWith(otherInfo);
    }

private:
    bn::intrusive_forward_list<coll::Collider> _colls;
    const bool _isTrigger;
};

} // namespace ut::game::cpnt
