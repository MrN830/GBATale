#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt
{
class NpcInput;
class WalkAnimCtrl;
} // namespace ut::game::cpnt

namespace ut::game::cpnt::inter
{

class CutsceneBasement3 final : public Interaction
{
public:
    CutsceneBasement3(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    auto onInteract(GameContext&) -> task::Task override;

private:
    ent::Entity* _toriel = nullptr;
    NpcInput* _torielInput = nullptr;
    WalkAnimCtrl* _torielWalk = nullptr;
};

} // namespace ut::game::cpnt::inter
