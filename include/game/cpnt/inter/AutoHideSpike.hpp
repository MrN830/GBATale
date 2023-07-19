#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class AutoHideSpike : public Interaction
{
public:
    AutoHideSpike(ent::Entity&, InteractionTriggers);

    auto getInteractionType() const -> bn::type_id_t override;

    void onInteract() override;
    void onInteractionCollisionExit() override;

    void render(GameContext&) override;

private:
    int _spikeReappearCountdown = -1;
};

} // namespace ut::game::cpnt::inter
