#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt
{
class Sprite;
class NpcInput;
class WalkAnimCtrl;
} // namespace ut::game::cpnt

namespace ut::game::cpnt::inter
{

class CutsceneBasement1Block final : public Interaction
{
public:
    CutsceneBasement1Block(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    auto onInteract(GameContext&) -> task::Task override;

private:
    ent::Entity* _frisk = nullptr;
    Sprite* _friskSpr = nullptr;
    NpcInput* _friskInput = nullptr;

    ent::Entity* _toriel = nullptr;
    Sprite* _torielSpr = nullptr;
    NpcInput* _torielInput = nullptr;
    WalkAnimCtrl* _torielWalk = nullptr;
};

} // namespace ut::game::cpnt::inter
