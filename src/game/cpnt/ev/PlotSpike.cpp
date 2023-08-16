#include "game/cpnt/ev/PlotSpike.hpp"

#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/ent/Entity.hpp"

namespace ut::game::cpnt::ev
{

namespace
{

enum SpikeGfxIdx
{
    SHOW = 0,
    HIDE = 1,
};

} // namespace

PlotSpike::PlotSpike(ent::Entity& entity, bool isEnabled, bool isAutoFire)
    : EventComponent(entity, bn::type_id<PlotSpike>(), isEnabled, isAutoFire)
{
}

void PlotSpike::awake(GameContext& ctx)
{
    const auto& state = ctx.state;

    const auto room = state.getRoom();
    const auto plot = state.getPlot();
    const auto& flags = state.getFlags();

    _spr = _entity.getComponent<cpnt::Sprite>();
    _collPack = _entity.getComponent<cpnt::ColliderPack>();

    BN_ASSERT(_spr != nullptr);
    BN_ASSERT(_collPack != nullptr);

    if ((room == RoomKind::ROOM_RUINS9 && flags.pushed_rock_4) ||
        (room == RoomKind::ROOM_RUINS11 && flags.pushed_rock_3) ||
        (room == RoomKind::ROOM_RUINS3 && (int)plot >= (int)GamePlot::SWITCH_PUZZLE_COMPLETE) ||
        (room == RoomKind::ROOM_RUINS14 && (int)plot >= (int)GamePlot::JUST_ONE_SWITCH_PUZZLE_SOLVED) ||
        (room == RoomKind::ROOM_RUINS15B && (int)plot >= (int)GamePlot::BLUE_SWITCH_FLIPPED) ||
        (room == RoomKind::ROOM_RUINS15C && (int)plot >= (int)GamePlot::RED_SWITCH_FLIPPED) ||
        (room == RoomKind::ROOM_RUINS15D && (int)plot >= (int)GamePlot::GREEN_SWITCH_FLIPPED) ||
        (room == RoomKind::ROOM_TUNDRA_SNOWPUZZ && (int)plot >= (int)GamePlot::UNDERSNOW_SWITCH_COMPLETE) ||
        (room == RoomKind::ROOM_TUNDRA_XOXOSMALL && (int)plot >= (int)GamePlot::SMALL_XO_PUZZLE_COMPLETE) ||
        (room == RoomKind::ROOM_TUNDRA_XOXOPUZZ && (int)plot >= (int)GamePlot::LARGE_XO_PUZZLE_COMPLETE))
    {
        hideSpike();
    }
}

auto PlotSpike::onEvent(GameContext&) -> task::Task
{
    co_return;
}

void PlotSpike::hideSpike()
{
    _spr->setGfxIdx(SpikeGfxIdx::HIDE);
    _collPack->setEnabled(false);
}

} // namespace ut::game::cpnt::ev
