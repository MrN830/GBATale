#pragma once

#include <type_traits>

#include <bn_fixed_point.h>
#include <bn_intrusive_forward_list.h>

#include "game/cpnt/Component.hpp"

namespace ut::game
{
struct GameContext;
}
namespace ut::game::sys
{
class EntityManager;
}

namespace ut::game::ent
{

namespace gen
{
enum class EntityId : uint16_t;
}

class Entity final : public bn::intrusive_forward_list_node_type
{
    // `EntityManager` deletes components from the cpnt heap on entity delete
    friend class sys::EntityManager;

public:
    Entity(gen::EntityId);

    void handleInput(GameContext& context);
    void update(GameContext& context);
    void render(GameContext& context);

public:
    template <typename TComponent>
        requires std::is_base_of_v<cpnt::Component, TComponent>
    void addComponent(TComponent& component);

    template <typename TComponent>
        requires std::is_base_of_v<cpnt::Component, TComponent>
    auto getComponent() -> TComponent*;

    template <typename TComponent>
        requires std::is_base_of_v<cpnt::Component, TComponent>
    auto getComponent() const -> const TComponent*;

public:
    auto getId() const -> gen::EntityId;

    bool isActive() const;
    bool isDestroyed() const;
    auto getPosition() const -> const bn::fixed_point&;

    void setActive(bool isActive);
    void setDestroyed(bool isDestroyed);
    void setPosition(const bn::fixed_point&);

private:
    bn::intrusive_forward_list<cpnt::Component> _components;
    bn::fixed_point _position;

    const gen::EntityId _id;

    bool _isActive : 1 = true;
    bool _isDestroyed : 1 = false;
};

} // namespace ut::game::ent

#include "game/ent/Entity.inl"
