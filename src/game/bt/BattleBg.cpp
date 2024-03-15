#include "game/bt/BattleBg.hpp"

#include "bn_regular_bg_items_bg_battle_boss.h"
#include "bn_regular_bg_items_bg_battle_normal.h"

#include "game/bt/bt_consts.hpp"

namespace ut::game::bt
{

BattleBg::BattleBg(BattleBgKind kind)
{
    switch (kind)
    {
    case BattleBgKind::EMPTY:
        break;
    case BattleBgKind::NORMAL:
        _bg = bn::regular_bg_items::bg_battle_normal.create_bg(0, 0);
        break;
    case BattleBgKind::BOSS:
        _bg = bn::regular_bg_items::bg_battle_boss.create_bg(0, 0);
        break;

    default:
        BN_ERROR("Invalid BattleBgKind=", (int)kind);
    }

    if (_bg)
        _bg->set_priority(consts::BG_BACKDROP_PRIORITY);
}

} // namespace ut::game::bt
