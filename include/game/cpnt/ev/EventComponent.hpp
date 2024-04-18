#pragma once

#include "game/cpnt/Component.hpp"

#include <cstdint>

#include "game/task/Task.hpp"

namespace ut::game::cpnt::ev
{

enum class EventComponentType : uint8_t;

class EventComponent : public Component
{
public:
    EventComponent(ent::Entity&, EventComponentType eventComponentType, bool isEnabled, bool isAutoFire);

    auto getEventComponentType() const -> EventComponentType;

    void update(GameContext&) override;

    virtual auto onEvent(GameContext&) -> task::Task = 0;

private:
    const bool _isAutoFire;
    bool _hasAutoFired = false;

    const EventComponentType _eventComponentType;
};

} // namespace ut::game::cpnt::ev
