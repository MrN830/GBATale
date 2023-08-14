#pragma once

#include "game/cpnt/ev/EventComponent.hpp"

namespace ut::game::cpnt::ev
{

class SetPieImage : public EventComponent
{
public:
    SetPieImage(ent::Entity&, bool isEnabled, bool isAutoFire);

    void onEvent(GameContext&) override;
};

} // namespace ut::game::cpnt::ev
