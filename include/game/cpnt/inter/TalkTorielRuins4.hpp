#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class TalkTorielRuins4 final : public Interaction
{
public:
    TalkTorielRuins4(ent::Entity&, bool isEnabled, InteractionTriggers);

    auto onInteract(GameContext&) -> task::Task override;

private:
    bool _hasTalked = false;
};

} // namespace ut::game::cpnt::inter
