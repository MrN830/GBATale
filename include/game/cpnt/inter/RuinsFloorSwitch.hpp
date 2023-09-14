#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class RuinsFloorSwitch : public Interaction
{
public:
    RuinsFloorSwitch(ent::Entity&, bool isEnabled, InteractionTriggers);

    auto onInteract(GameContext&) -> task::Task override;

private:
    bool isPressed = false;
};

} // namespace ut::game::cpnt::inter
