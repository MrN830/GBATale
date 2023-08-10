#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt
{
class Sprite;
class ColliderPack;
} // namespace ut::game::cpnt

namespace ut::game::cpnt::inter
{

class PlotSpike final : public Interaction
{
public:
    PlotSpike(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

public:
    void hideSpike();

private:
    Sprite* _spr;
    ColliderPack* _collPack;
};

} // namespace ut::game::cpnt::inter
