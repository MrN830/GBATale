#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class RuinsFloorSwitch : public Interaction
{
public:
    RuinsFloorSwitch(ent::Entity&, InteractionTriggers);

    void onInteract() override;

private:
    bool isPressed = false;
};

} // namespace ut::game::cpnt::inter
