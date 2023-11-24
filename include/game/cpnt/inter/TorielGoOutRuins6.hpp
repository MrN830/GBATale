#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class TorielGoOutRuins6 final : public Interaction
{
public:
    TorielGoOutRuins6(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    auto onInteract(GameContext&) -> task::Task override;
};

} // namespace ut::game::cpnt::inter
