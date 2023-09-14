#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class AutoHideSpike : public Interaction
{
public:
    AutoHideSpike(ent::Entity&, bool isEnabled, InteractionTriggers);

    auto onInteract(GameContext&) -> task::Task override;
    void onInteractionCollisionExit(GameContext&) override;

    void render(GameContext&) override;

private:
    int _spikeReappearCountdown = -1;
};

} // namespace ut::game::cpnt::inter
