#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class TorielGoOutRuins5 final : public Interaction
{
public:
    TorielGoOutRuins5(ent::Entity&, bool isEnabled, InteractionTriggers);

    auto onInteract(GameContext&) -> task::Task override;
};

} // namespace ut::game::cpnt::inter
