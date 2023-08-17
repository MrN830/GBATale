#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class MouseSqueak : public Interaction
{
public:
    MouseSqueak(ent::Entity&, bool isEnabled, InteractionTriggers);

    auto onInteract(GameContext&) -> task::Task override;
};

} // namespace ut::game::cpnt::inter
