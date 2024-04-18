#include "game/bt/vfx/BattleTimedVfx.hpp"

namespace ut::game::bt::vfx
{

BattleTimedVfx::~BattleTimedVfx() = default;

BattleTimedVfx::BattleTimedVfx(int destroyTicks) : _destroyCountdown(destroyTicks)
{
    BN_ASSERT(destroyTicks > 0, "Non-positive destroyTicks=", destroyTicks);
}

void BattleTimedVfx::render()
{
    if (_destroyCountdown > 0)
        --_destroyCountdown;

    renderVfx();
}

bool BattleTimedVfx::shouldDestroyed() const
{
    return _destroyCountdown <= 0;
}

} // namespace ut::game::bt
