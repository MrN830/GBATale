
namespace ut::game::ent
{

template <typename TComponent>
    requires std::is_base_of_v<cpnt::Component, TComponent>
void Entity::addComponent(TComponent& component)
{
    _components.push_front(component);
}

template <typename TComponent>
    requires std::is_base_of_v<cpnt::Component, TComponent>
auto Entity::getComponent() -> TComponent*
{
    for (auto& component : _components)
        if (component.getType() == bn::type_id<TComponent>())
            return static_cast<TComponent*>(&component);

    return nullptr;
}

template <typename TComponent>
    requires std::is_base_of_v<cpnt::Component, TComponent>
auto Entity::getComponent() const -> const TComponent*
{
    for (const auto& component : _components)
        if (component.getType() == bn::type_id<TComponent>())
            return static_cast<const TComponent*>(&component);

    return nullptr;
}

} // namespace ut::game::ent
