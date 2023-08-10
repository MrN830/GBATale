#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class RuinsColorSwitchHelp final : public Interaction
{
public:
    RuinsColorSwitchHelp(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    void onInteract(GameContext&) override;
};

} // namespace ut::game::cpnt::inter
