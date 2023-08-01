#pragma once

#include "game/cpnt/Component.hpp"

namespace ut::game::cpnt::ev
{

class EventComponent : public Component
{
public:
    EventComponent(ent::Entity&, bn::type_id_t eventComponentType, bool isEnabled, bool isAutoFire);

    auto getEventComponentType() const -> bn::type_id_t;

    void update(GameContext&) override;

    virtual void onEvent(GameContext&) = 0;

private:
    const bool _isAutoFire;
    bool _hasAutoFired = false;

    const bn::type_id_t _eventComponentType;
};

} // namespace ut::game::cpnt::ev
