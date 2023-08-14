#pragma once

#include "game/cpnt/inter/Interaction.hpp"

namespace ut::game::cpnt::inter
{

class TalkFroggit final : public Interaction
{
public:
    TalkFroggit(ent::Entity&, bool isEnabled, InteractionTriggers);

    void awake(GameContext&) override;

    void onInteract(GameContext&) override;
    void update(GameContext&) override;

private:
    bool _isDialogSkipAdviceFroggit : 1 = false;

    bool _isDialogSkipAdviceStarted : 1 = false;
    bool _isDialogSkipAdviceChanged : 1 = false;
    uint32_t _prevDialogSkippedCount = 0;
};

} // namespace ut::game::cpnt::inter
