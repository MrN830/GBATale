#pragma once

#include "game/cpnt/ev/EventComponent.hpp"

namespace ut::game::cpnt
{
class NpcInput;
class WalkAnimCtrl;
} // namespace ut::game::cpnt

namespace ut::game::cpnt::ev
{

class CutsceneRuins5 final : public EventComponent
{
public:
    CutsceneRuins5(ent::Entity&, bool isEnabled, bool isAutoFire);

    void awake(GameContext&) override;

    void update(GameContext&) override;

    auto onEvent(GameContext&) -> task::Task override;

private:
    ent::Entity* _frisk = nullptr;
    NpcInput* _torielInput = nullptr;
    WalkAnimCtrl* _torielWalk = nullptr;

    int _torielPauseCountdown = -1;
    bool _torielTooFarTrigger = false;
};

} // namespace ut::game::cpnt::ev
