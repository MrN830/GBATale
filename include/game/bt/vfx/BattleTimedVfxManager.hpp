#pragma once

#include <type_traits>

#include <bn_best_fit_allocator.h>
#include <bn_intrusive_forward_list.h>

#include "game/bt/vfx/BattleTimedVfx.hpp"

namespace ut::game::bt::vfx
{

class BattleTimedVfxManager
{
public:
    BattleTimedVfxManager();
    ~BattleTimedVfxManager();

    void render();

    template <typename TVfx, typename... TParams>
        requires std::is_base_of_v<BattleTimedVfx, TVfx>
    void addVfx(TParams&&... params)
    {
        TVfx& vfx = _vfxHeap.create<TVfx>(std::forward<TParams>(params)...);
        _vfxes.push_front(vfx);
    }

private:
    void removeDestroyed(bool forceRemoveAll);

private:
    alignas(4) uint8_t _vfxBuffer[512];
    bn::best_fit_allocator _vfxHeap;
    bn::intrusive_forward_list<BattleTimedVfx> _vfxes;
};

} // namespace ut::game::bt
