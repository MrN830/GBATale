#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class SavePoint final : public Interaction
{
public:
    SavePoint(ent::Entity&, bool isEnabled, InteractionTriggers);

    void onInteract(GameContext&) override;
};

} // namespace ut::game::cpnt::inter
