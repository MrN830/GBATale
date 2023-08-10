#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class RuinsColorSwitch final : public Interaction
{
public:
    RuinsColorSwitch(ent::Entity&, bool isEnabled, InteractionTriggers);

    void onInteract(GameContext&) override;
};

} // namespace ut::game::cpnt::inter
