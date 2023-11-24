#pragma once

#include "game/cpnt/ev/EventComponent.hpp"

namespace ut::game::cpnt::ev
{

class StalkerFlowey final : public EventComponent
{
public:
    StalkerFlowey(ent::Entity&, bool isEnabled, bool isAutoFire);

    void update(GameContext&) override;

    auto onEvent(GameContext&) -> task::Task override;

private:
    bool isOnScreen(GameContext& ctx) const;

private:
    bool _isHideStarted = false;
};

} // namespace ut::game::cpnt::ev
