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
    void addCollider(TColl& coll);

    bool isTrigger() const;

    bool isCollideWith(const ColliderPack& other) const;
    bool isCollideWith(const coll::CollInfo& other) const;

    template <typename CInfo>
    bool isCollideWith(const CInfo& other) const;

private:
    bn::intrusive_forward_list<coll::Collider> _colls;
    const bool _isTrigger;
};

} // namespace ut::game::cpnt

#include "game/cpnt/ColliderPack.inl"
