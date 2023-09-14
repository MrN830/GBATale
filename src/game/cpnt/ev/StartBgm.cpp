#include "game/cpnt/ev/StartBgm.hpp"

#include "asset/Bgm.hpp"
#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"

namespace ut::game::cpnt::ev
{

StartBgm::StartBgm(ent::Entity& entity, bool isEnabled, bool isAutoFire)
    : EventComponent(entity, bn::type_id<StartBgm>(), isEnabled, isAutoFire)
{
}

auto StartBgm::onEvent(GameContext& ctx) -> task::Task
{
    if (asset::Bgm::isPlaying())
        co_return;

    auto& state = ctx.state;
    const auto room = state.getRoom();
    const auto& flags = state.getFlags();

    auto bgmKind = asset::BgmKind::NONE;

    switch (room)
    {
    case RoomKind::ROOM_RUINS1:
    case RoomKind::ROOM_RUINS5:
    case RoomKind::ROOM_RUINS6:
    case RoomKind::ROOM_RUINS7:
    case RoomKind::ROOM_RUINS12A:
    case RoomKind::ROOM_RUINS16:
    case RoomKind::ROOM_RUINS19:
        bgmKind = asset::BgmKind::RUINS;
        break;

    case RoomKind::ROOM_TORHOUSE1:
        if (flags.status_toriel != game::GameFlags::StatusToriel::KILLED)
            bgmKind = asset::BgmKind::HOME;
        break;

    case RoomKind::ROOM_BASEMENT1:
        if (!flags.true_pacifist)
            bgmKind = asset::BgmKind::BASEMENT;
        break;

    default:
        BN_ERROR("StartBgm in invalid room=", (int)room);
    }

    state.setWorldBgm(bgmKind);
    asset::Bgm::play(bgmKind);

    co_return;
}

} // namespace ut::game::cpnt::ev
