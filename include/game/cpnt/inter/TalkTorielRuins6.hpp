#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class TalkTorielRuins6 final : public Interaction
{
public:
    TalkTorielRuins6(ent::Entity&, bool isEnabled, InteractionTriggers);

    auto onInteract(GameContext&) -> task::Task override;
};

} // namespace ut::game::cpnt::inter
