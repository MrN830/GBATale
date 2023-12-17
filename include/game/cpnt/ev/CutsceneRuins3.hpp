#pragma once

#include "game/cpnt/ev/EventComponent.hpp"

namespace ut::game::ent
{
class Entity;
}

namespace ut::game::cpnt::ev
{

class CutsceneRuins3 final : public EventComponent
{
public:
    CutsceneRuins3(ent::Entity&, bool isEnabled, bool isAutoFire);

    void awake(GameContext&) override;

    void update(GameContext&) override;

    auto onEvent(GameContext&) -> task::Task override;

private:
    ent::Entity* _frisk;

    bool _hasFired = false;
};

} // namespace ut::game::cpnt::ev
