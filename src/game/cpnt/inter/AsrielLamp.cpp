#include "game/cpnt/inter/AsrielLamp.hpp"

#include <bn_music.h>
#include <bn_sound_item.h>

#include "asset/Bgm.hpp"
#include "asset/SfxKind.hpp"
#include "game/GameContext.hpp"
#include "game/GameFlags.hpp"
#include "game/GameState.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/WorldBg.hpp"

#include "gen/EntityId.hpp"

#include "bn_bg_palette_items_pal_asrielroom_dark.h"
#include "bn_bg_palette_items_pal_asrielroom_light.h"

namespace ut::game::cpnt::inter
{

static constexpr int BGM_FADE_WAIT_UPDATE = 20;

AsrielLamp::AsrielLamp(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<AsrielLamp>(), isEnabled, triggers)
{
    BN_ASSERT(entity.getId() == ent::gen::EntityId::lamp);
}

void AsrielLamp::awake(GameContext& ctx)
{
    _isTruePacifist = ctx.state.getFlags().true_pacifist;

    // initial swap Bgm (for `torhouse3`)
    if (!_isTruePacifist)
    {
        using namespace ut::asset;

        const auto prevBgm = Bgm::getPlayingBgmKind();
        if (prevBgm == BgmKind::FALLEN_DOWN)
            _bgmFade = asset::BgmVolumeToAction(BGM_FADE_WAIT_UPDATE, 0);
        else if (prevBgm == BgmKind::HOME_MUSIC_BOX)
            toggleMusicBoxHomeBgm();
    }
}

auto AsrielLamp::onInteract(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    Interaction::onInteract(ctx);

    getSfx(SfxKind::SWITCH_TRIGGER)->play();

    _isLightOn = !_isLightOn;

    // swap Bgm
    if (!_isTruePacifist)
    {
        const auto prevBgm = Bgm::getPlayingBgmKind();
        if (prevBgm == BgmKind::HOME || prevBgm == BgmKind::HOME_MUSIC_BOX)
        {
            toggleMusicBoxHomeBgm();
        }
        else if (prevBgm == BgmKind::FALLEN_DOWN)
        {
            if (_isLightOn)
                _bgmFade = asset::BgmVolumeToAction(BGM_FADE_WAIT_UPDATE, 0);
            else
                _bgmFade = asset::BgmVolumeToAction(BGM_FADE_WAIT_UPDATE, 1);
        }
        else
            BN_ERROR("Invalid prevBgm=", (int)prevBgm);
    }

    // swap BG palette
    const auto& bgPal =
        (_isLightOn ? bn::bg_palette_items::pal_asrielroom_light : bn::bg_palette_items::pal_asrielroom_dark);
    ctx.worldBg.setBgPalettes(bgPal, bn::nullopt, bn::nullopt);

    co_return;
}

void AsrielLamp::update(GameContext& ctx)
{
    Interaction::update(ctx);

    if (_bgmFade.has_value())
    {
        if (!_bgmFade->done())
            _bgmFade->update();

        if (_bgmFade->done())
            _bgmFade.reset();
    }
}

void AsrielLamp::toggleMusicBoxHomeBgm()
{
    using namespace ut::asset;

    const auto prevBgm = Bgm::getPlayingBgmKind();
    BN_ASSERT(prevBgm == BgmKind::HOME || prevBgm == BgmKind::HOME_MUSIC_BOX);

    const int musicPos = bn::music::position();
    const auto nextBgm = (prevBgm == BgmKind::HOME ? BgmKind::HOME_MUSIC_BOX : BgmKind::HOME);
    Bgm::play(nextBgm);
    bn::music::set_position(musicPos);
}

} // namespace ut::game::cpnt::inter
