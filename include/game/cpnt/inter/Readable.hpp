#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class Readable final : public Interaction
{
public:
    Readable(ent::Entity&, bool isEnabled, InteractionTriggers);

    void onInteract(GameContext&) override;

private:
    int _readCount = 0;
};

} // namespace ut::game::cpnt::inter
