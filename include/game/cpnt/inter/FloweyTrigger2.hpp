#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt
{
class SpriteAnim;
}

namespace ut::game::cpnt::inter
{

class FloweyTrigger2 final : public Interaction
{
public:
    FloweyTrigger2(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    auto onInteract(GameContext&) -> task::Task override;

private:
    ent::Entity* _flowey = nullptr;
    SpriteAnim* _floweyAnim = nullptr;
};

} // namespace ut::game::cpnt::inter
