#pragma once

#include "game/cpnt/ev/EventComponent.hpp"

namespace ut::game::cpnt::ev
{

class TimedDestroy final : public EventComponent
{
public:
    TimedDestroy(ent::Entity&, bool isEnabled, bool isAutoFire);

    auto onEvent(GameContext&) -> task::Task override;

public:
    void setDestroyTicks(int ticks);

private:
    int _destroyTicks = 0;

    bool _hasFired = false;
};

} // namespace ut::game::cpnt::ev
