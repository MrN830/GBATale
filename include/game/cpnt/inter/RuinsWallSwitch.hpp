#pragma once

#include "game/cpnt/inter/RuinsSpikeDisabler.hpp"

namespace ut::game::cpnt::inter
{

class RuinsWallSwitch final : public RuinsSpikeDisabler
{
public:
    RuinsWallSwitch(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    auto onInteract(GameContext&) -> task::Task override;

private:
    void setPressed(bool isPressed);

private:
    bool _isPressed = false;
};

} // namespace ut::game::cpnt::inter
