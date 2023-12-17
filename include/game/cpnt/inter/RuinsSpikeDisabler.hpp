#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class RuinsSpikeDisabler : public Interaction
{
public:
    virtual ~RuinsSpikeDisabler() = 0;

    RuinsSpikeDisabler(ent::Entity&, bn::type_id_t interactionType, bool isEnabled, InteractionTriggers);

protected:
    void hideAllSpikesInRoom(GameContext&);
};

} // namespace ut::game::cpnt::inter
