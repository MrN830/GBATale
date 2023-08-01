#include "game/sys/RoomChanger.hpp"

#include "asset/Bgm.hpp"
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

        BN_ASSERT(ctx.interactStack.top() == InteractState::ROOM_CHANGE);
        ctx.interactStack.pop();
    }
}

bool RoomChanger::isChanging() const
{
    return _room != RoomKind::NONE;
}

void RoomChanger::reqChange(const mtile::Warp& warp, GameContext& ctx)
{
    BN_ASSERT(warp.room != RoomKind::NONE);

    ctx.interactStack.push(InteractState::ROOM_CHANGE);

    _countdown = FO_FRAMES;
    ctx.fadeMngr.startFadeOut(FO_FRAMES);
    bgmFadeOut(warp, ctx);

    _room = warp.room;
    ctx.warpId = warp.warpId;
}

void RoomChanger::instantChange(const mtile::Warp& warp, GameContext& ctx)
{
    BN_ASSERT(warp.room != RoomKind::NONE);

    _room = warp.room;
    ctx.warpId = warp.warpId;

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

void RoomChanger::bgmFadeOut(const mtile::Warp& warp, GameContext& ctx)
{
    if (!warp.isBgmFadeOut)
        return;
    if (ctx.state.getFlags().true_pacifist)
        return;

    // TODO: On Undyne chase, DO NOT fade-out BGM

    if (asset::Bgm::isPlaying())
        asset::Bgm::fadeOutAndStash(FO_FRAMES);
}

} // namespace ut::game::sys
