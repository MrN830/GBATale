#include "game/cpnt/ev/StartBgm.hpp"

#include <bn_music.h>
#include <bn_optional.h>

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
    bn::optional<bn::fixed> volumeOverride;
    bn::optional<bn::fixed> tempoOverride; // DirectSound only
    bn::optional<bn::fixed> pitchOverride; // DirectSound only

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
        else
        {
            bgmKind = asset::BgmKind::FALLEN_DOWN;
            volumeOverride = 0;
            tempoOverride = 0.5;
            pitchOverride = 0.5;
        }
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

    // overrides
    if (volumeOverride.has_value())
        asset::Bgm::setVolume(*volumeOverride);
    if (tempoOverride.has_value())
    {
        BN_ASSERT(bn::music::playing());
        bn::music::set_tempo(*tempoOverride);
    }
    if (pitchOverride.has_value())
    {
        BN_ASSERT(bn::music::playing());
        bn::music::set_pitch(*pitchOverride);
    }

    co_return;
}

} // namespace ut::game::cpnt::ev
