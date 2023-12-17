#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class RuinsTorielCall final : public Interaction
{
public:
    RuinsTorielCall(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    auto onInteract(GameContext&) -> task::Task override;
};

} // namespace ut::game::cpnt::inter
