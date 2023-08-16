#pragma once

#include "game/cpnt/ev/EventComponent.hpp"

namespace ut::game::cpnt::ev
{

class StartBgm : public EventComponent
{
public:
    StartBgm(ent::Entity&, bool isEnabled, bool isAutoFire);

    auto onEvent(GameContext&) -> task::Task override;
};

} // namespace ut::game::cpnt::ev
