#pragma once

#include "game/cpnt/inter/Interaction.hpp"

#include <bn_optional.h>

#include "game/ent/EntityActions.hpp"

namespace ut::game::cpnt::inter
{

class HoleUp : public Interaction
{
public:
    HoleUp(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    auto onInteract(GameContext&) -> task::Task override;

    void update(GameContext&) override;

private:
    void initFriskMoveAction();

private:
    ent::Entity* _frisk;

    int _delayCountdown = -1;

    bn::optional<ent::EntityMoveToAction> _friskMoveAction;
};

} // namespace ut::game::cpnt::inter
