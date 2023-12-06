#pragma once

#include "game/cpnt/ev/EventComponent.hpp"

namespace ut::game::cpnt::ev
{

class CutsceneTorhouse1 final : public EventComponent
{
public:
    CutsceneTorhouse1(ent::Entity&, bool isEnabled, bool isAutoFire);

    void awake(GameContext&) override;

    auto onEvent(GameContext&) -> task::Task override;
};

} // namespace ut::game::cpnt::ev
