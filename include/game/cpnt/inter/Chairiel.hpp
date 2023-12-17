#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt
{
class Sprite;
}

namespace ut::game::cpnt::inter
{

class Chairiel final : public Interaction
{
public:
    Chairiel(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    auto onInteract(GameContext&) -> task::Task override;

private:
    Sprite* _chairielSpr = nullptr;

    bool _isTorielSitting = false;
};

} // namespace ut::game::cpnt::inter
