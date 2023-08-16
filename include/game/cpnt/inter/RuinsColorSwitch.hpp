#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class RuinsColorSwitch final : public Interaction
{
public:
    RuinsColorSwitch(ent::Entity&, bool isEnabled, InteractionTriggers);

    auto onInteract(GameContext&) -> task::Task override;
};

} // namespace ut::game::cpnt::inter
