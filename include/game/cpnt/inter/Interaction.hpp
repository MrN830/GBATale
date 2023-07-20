#pragma once

#include "game/cpnt/Component.hpp"

#include "game/cpnt/inter/InteractionTriggers.hpp"

namespace ut::game::cpnt::inter
{

class Interaction : public Component
{
public:
    virtual ~Interaction() = 0;
    Interaction(ent::Entity&, bn::type_id_t interactionType, InteractionTriggers);

    auto getInteractionType() const -> bn::type_id_t;

    virtual void onInteract();
    virtual void onInteractionCollisionExit(){};

    void update(GameContext&) override;

public:
    auto getTriggers() const -> InteractionTriggers;
    void setTriggers(InteractionTriggers);

private:
    InteractionTriggers _triggers;

    bool _isColliding = false;

    const bn::type_id_t _interactionType;
};

} // namespace ut::game::cpnt::inter
