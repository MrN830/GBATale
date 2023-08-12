#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class ItemPickup final : public Interaction
{
public:
    ItemPickup(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    void onInteract(GameContext&) override;

private:
    void destroyIfAlreadyGot(GameContext&);
};

} // namespace ut::game::cpnt::inter
