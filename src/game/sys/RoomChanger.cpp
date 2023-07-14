#include "game/sys/RoomChanger.hpp"

#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/sys/EntityManager.hpp"
#include "game/sys/FadeManager.hpp"
#include "game/sys/WorldBg.hpp"
#include "mtile/MTilemap.hpp"

namespace ut::game::sys
{

namespace
{
constexpr int FO_FRAMES = 8;
constexpr int FI_FRAMES = 12;
} // namespace

RoomChanger::RoomChanger() : _room(RoomKind::NONE)
{
}

void RoomChanger::update(GameContext& ctx)
{
    if (!isChanging())
        return;

    if (--_countdown < 0)
    {
        changeRoom(ctx);

        ctx.fadeMngr.startFadeIn(FI_FRAMES);

        _room = RoomKind::NONE;
    }
}

bool RoomChanger::isChanging() const
{
    return _room != RoomKind::NONE;
}

void RoomChanger::reqChange(RoomKind room, mtile::WarpId warpId, GameContext& ctx)
{
    BN_ASSERT(room != RoomKind::NONE);

    _room = room;
    ctx.warpId = warpId;

    _countdown = FO_FRAMES;
    ctx.fadeMngr.startFadeOut(FO_FRAMES);
}

void RoomChanger::instantChange(RoomKind room, mtile::WarpId warpId, GameContext& ctx)
{
    BN_ASSERT(room != RoomKind::NONE);

    _room = room;
    ctx.warpId = warpId;

    changeRoom(ctx);

    _room = RoomKind::NONE;
}

void RoomChanger::changeRoom(GameContext& ctx)
{
    ctx.state.setRoom(_room);
    ctx.entMngr.reloadRoom(ctx);

    const auto* mTilemap = getRoomMTilemap(_room);
    BN_ASSERT(mTilemap != nullptr, "No mTilemap for room=", (int)_room);
    ctx.worldBg.setMTilemap(*mTilemap);
}

} // namespace ut::game::sys
