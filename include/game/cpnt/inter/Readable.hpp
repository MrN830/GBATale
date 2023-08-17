#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class Readable final : public Interaction
{
public:
    Readable(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    auto onInteract(GameContext&) -> task::Task override;

private:
    void dropCandyDish(GameContext&);

private:
    int _readCount = 0;
};

} // namespace ut::game::cpnt::inter
