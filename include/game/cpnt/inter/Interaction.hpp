#pragma once

#include "game/cpnt/Component.hpp"

#include "game/cpnt/inter/InteractionTriggers.hpp"

namespace ut::game::cpnt::inter
{

class Interaction : public Component
{
public:
    Interaction(ent::Entity&, InteractionTriggers);

    auto getType() const -> bn::type_id_t final;
    virtual auto getInteractionType() const -> bn::type_id_t = 0;

    virtual void onInteract();
    virtual void onInteractionCollisionExit(){};

    void update(GameContext&) override;

public:
    auto getTriggers() const -> InteractionTriggers;
    void setTriggers(InteractionTriggers);

private:
    InteractionTriggers _triggers;

    bool _isColliding = false;
};

} // namespace ut::game::cpnt::inter
