#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class TalkTorielRuins3 final : public Interaction
{
public:
    TalkTorielRuins3(ent::Entity&, bool isEnabled, InteractionTriggers);

    auto onInteract(GameContext&) -> task::Task override;

public:
    void setTalked(bool hasTalked);

private:
    bool _hasTalked = false;
};

} // namespace ut::game::cpnt::inter
