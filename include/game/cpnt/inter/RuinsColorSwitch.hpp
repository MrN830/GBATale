#pragma once

#include "game/cpnt/inter/RuinsSpikeDisabler.hpp"

namespace ut::game::cpnt::inter
{

class RuinsColorSwitch final : public RuinsSpikeDisabler
{
public:
    RuinsColorSwitch(ent::Entity&, bool isEnabled, InteractionTriggers);

    auto onInteract(GameContext&) -> task::Task override;
};

} // namespace ut::game::cpnt::inter
