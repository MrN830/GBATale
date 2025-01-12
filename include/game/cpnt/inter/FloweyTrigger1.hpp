#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class FloweyTrigger1 final : public Interaction
{
public:
    FloweyTrigger1(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    auto onInteract(GameContext&) -> task::Task override;

private:
    ent::Entity* _flowey = nullptr;
    ent::Entity* _toriel = nullptr;

    bool _noFloweyEncounter = false;
};

} // namespace ut::game::cpnt::inter
