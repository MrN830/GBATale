#pragma once

#include "game/cpnt/Component.hpp"

#include "game/cpnt/inter/InteractionTriggers.hpp"
#include "game/task/Task.hpp"

namespace ut::game::cpnt::inter
{

enum class InteractionType : uint8_t;

class Interaction : public Component
{
public:
    virtual ~Interaction() = 0;
    Interaction(ent::Entity&, InteractionType interactionType, bool isEnabled, InteractionTriggers);

    auto getInteractionType() const -> InteractionType;

    virtual auto onInteract(GameContext&) -> task::Task;
    virtual void onInteractionCollisionExit(GameContext&){};

    void update(GameContext&) override;

public:
    auto getTriggers() const -> InteractionTriggers;
    void setTriggers(InteractionTriggers);

private:
    InteractionTriggers _triggers;

    bool _isColliding = false;

    const InteractionType _interactionType;
};

} // namespace ut::game::cpnt::inter
