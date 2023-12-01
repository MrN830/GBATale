#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt
{
class Sprite;
class SpriteAnim;
class NpcInput;
class WalkAnimCtrl;
} // namespace ut::game::cpnt

namespace ut::game::cpnt::inter
{

class CutsceneRuins19 final : public Interaction
{
public:
    CutsceneRuins19(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    auto onInteract(GameContext&) -> task::Task override;

private:
    ent::Entity* _toriel = nullptr;
    Sprite* _torielSpr = nullptr;
    SpriteAnim* _torielSprAnim = nullptr;
    NpcInput* _torielInput = nullptr;
    WalkAnimCtrl* _torielWalk = nullptr;
};

} // namespace ut::game::cpnt::inter
