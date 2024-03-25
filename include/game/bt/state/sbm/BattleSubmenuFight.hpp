#pragma once

#include "game/bt/state/sbm/BattleSubmenu.hpp"

#include <bn_bitset.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "game/bt/HpBar.hpp"

#include "game/bt/bt_consts.hpp"

namespace ut::game::bt::state::sbm
{

class BattleSubmenuFight final : public BattleSubmenu
{
public:
    BattleSubmenuFight(BattleMenu& priMenu);
    ~BattleSubmenuFight();

    auto handleInput() -> BattleSubmenuType override;

private:
    void updateHeartPos();

private:
    bn::vector<bn::sprite_ptr, consts::MAX_MOBS * 4> _nameSprs;
    bn::vector<HpBar, consts::MAX_MOBS> _hpBars;

    bn::sprite_ptr _heartSpr;

    // [0..2] if mob of index `i` is active or not
    bn::bitset<8> _mobActiveFlags;
};

} // namespace ut::game::bt::state::sbm
