#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class SavePoint final : public Interaction
{
public:
    SavePoint(ent::Entity&, InteractionTriggers);

    void onInteract() override;
};

} // namespace ut::game::cpnt::inter
