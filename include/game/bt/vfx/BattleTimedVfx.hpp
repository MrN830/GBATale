#pragma once

#include <bn_intrusive_forward_list.h>

namespace ut::game::bt::vfx
{

class BattleTimedVfx : public bn::intrusive_forward_list_node_type
{
public:
    virtual ~BattleTimedVfx() = 0;

    BattleTimedVfx(int destroyTicks);

    void render();

    bool shouldDestroyed() const;

protected:
    virtual void renderVfx() = 0;

private:
    int _destroyCountdown;
};

} // namespace ut::game::bt
