#pragma once

#include "game/cpnt/ev/EventComponent.hpp"

namespace ut::game::cpnt::ev
{

class CutsceneRuins4 final : public EventComponent
{
public:
    CutsceneRuins4(ent::Entity&, bool isEnabled, bool isAutoFire);

    void awake(GameContext&) override;

    auto onEvent(GameContext&) -> task::Task override;
};

} // namespace ut::game::cpnt::ev
