#pragma once

#include "game/cpnt/ev/EventComponent.hpp"

namespace ut::game::cpnt
{
class Sprite;
class ColliderPack;
} // namespace ut::game::cpnt

namespace ut::game::cpnt::ev
{

class PlotSpike final : public EventComponent
{
public:
    PlotSpike(ent::Entity&, bool isEnabled, bool isAutoFire);

    void awake(GameContext&) override;

    auto onEvent(GameContext&) -> task::Task override;

public:
    void hideSpike();

private:
    Sprite* _spr;
    ColliderPack* _collPack;
};

} // namespace ut::game::cpnt::ev
