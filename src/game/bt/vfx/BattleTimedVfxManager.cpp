#include "game/bt/vfx/BattleTimedVfxManager.hpp"

namespace ut::game::bt::vfx
{

BattleTimedVfxManager::BattleTimedVfxManager() : _vfxHeap(_vfxBuffer, sizeof(_vfxBuffer))
{
}

BattleTimedVfxManager::~BattleTimedVfxManager()
{
    removeDestroyed(true);
}

void BattleTimedVfxManager::render()
{
    for (auto& vfx : _vfxes)
        vfx.render();

    removeDestroyed(false);
}

void BattleTimedVfxManager::removeDestroyed(bool forceRemoveAll)
{
    for (auto beforeIt = _vfxes.before_begin(), it = _vfxes.begin(); it != _vfxes.end();)
    {
        auto& vfx = *it;

        // Do not destroy `vfx` when destroy isn't requested
        if (!forceRemoveAll && !vfx.shouldDestroyed())
        {
            beforeIt = it;
            ++it;
            continue;
        }

        it = _vfxes.erase_after(beforeIt);
        _vfxHeap.destroy(vfx);
    }
}

} // namespace ut::game::bt
