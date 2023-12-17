#pragma once

#include "game/cpnt/inter/Interaction.hpp"

#include <bn_optional.h>

#include "asset/BgmActions.hpp"

namespace ut::game::cpnt::inter
{

class AsrielLamp final : public Interaction
{
public:
    AsrielLamp(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    auto onInteract(GameContext&) -> task::Task override;

    void update(GameContext&) override;

private:
    void toggleMusicBoxHomeBgm();

private:
    bn::optional<asset::BgmVolumeToAction> _bgmFade;

    bool _isTruePacifist = false;
    bool _isLightOn = true;
};

} // namespace ut::game::cpnt::inter
