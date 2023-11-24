#include "asset/Bgm.hpp"

#include "asset/BgmActions.hpp"

#include <bn_array.h>
#include <bn_assert.h>
#include <bn_audio.h>
#include <bn_dmg_music.h>
#include <bn_dmg_music_position.h>
#include <bn_music.h>
#include <bn_optional.h>

#include "bn_dmg_music_items_once_upon_a_time.h"
#include "bn_dmg_music_items_your_best_friend.h"
#include "bn_music_items.h"

namespace ut::asset
{

static constexpr bn::array<Bgm, (int)BgmKind::TOTAL_COUNT> BGMS = {
    Bgm{BgmKind::NONE, nullptr, nullptr},
    {BgmKind::ONCE_UPON_A_TIME, nullptr, &bn::dmg_music_items::once_upon_a_time},
    {BgmKind::START_MENU, &bn::music_items::menu, nullptr, 1.0 / 6},
    {BgmKind::YOUR_BEST_FRIEND, nullptr, &bn::dmg_music_items::your_best_friend},
    {BgmKind::FALLEN_DOWN, &bn::music_items::fallen_down, nullptr},
    {BgmKind::RUINS, &bn::music_items::ruins, nullptr, 1.0 / 4},
    {BgmKind::BASEMENT, &bn::music_items::basement, nullptr, 1.0 / 3},
    {BgmKind::UWA_SO_TEMPERATE, nullptr, nullptr},
    {BgmKind::ANTICIPATION, &bn::music_items::anticipation, nullptr},
    {BgmKind::UNNECESSARY_TENSION, &bn::music_items::unnecessary_tension, nullptr, 1.0 / 4},
    {BgmKind::ENEMY_APPROACHING, &bn::music_items::enemy_approaching, nullptr},
    {BgmKind::GHOST_FIGHT, &bn::music_items::ghost_fight, nullptr},
    {BgmKind::DETERMINATION, &bn::music_items::determination, nullptr},
    {BgmKind::HOME, &bn::music_items::home, nullptr, 1.0 / 4},
    {BgmKind::HOME_MUSIC_BOX, &bn::music_items::home_music_box, nullptr, 1.0 / 4},
    {BgmKind::HEARTACHE, &bn::music_items::heartache, nullptr},
    {BgmKind::SANS, nullptr, nullptr},
    {BgmKind::NYEH_HEH_HEH, nullptr, nullptr},
    {BgmKind::SNOWY, nullptr, nullptr},
    {BgmKind::UWA_SO_HOLIDAY, nullptr, nullptr},
    {BgmKind::DOGBASS, nullptr, nullptr},
    {BgmKind::MYSTERIOUS_PLACE, nullptr, nullptr},
    {BgmKind::DOGSONG, nullptr, nullptr},
    {BgmKind::SNOWDIN_TOWN, nullptr, nullptr},
    {BgmKind::SHOP, nullptr, nullptr},
    {BgmKind::BONETROUSLE, nullptr, nullptr},
    {BgmKind::DATING_START, nullptr, nullptr},
    {BgmKind::DATING_TENSE, nullptr, nullptr},
    {BgmKind::DATING_FIGHT, nullptr, nullptr},
    {BgmKind::PREMONITION, nullptr, nullptr},
    {BgmKind::DANGER_MYSTERY, nullptr, nullptr},
    {BgmKind::UNDYNE, nullptr, nullptr},
    {BgmKind::WATERFALL, nullptr, nullptr},
    {BgmKind::RUN, nullptr, nullptr},
    {BgmKind::QUIET_WATER, nullptr, nullptr},
    {BgmKind::MEMORY, nullptr, nullptr},
    {BgmKind::BIRD_THAT_CARRIES_YOU_OVER_A_DISPROPORTIONATELY_SMALL_GAP, nullptr, nullptr},
    {BgmKind::DUMMY, nullptr, nullptr},
    {BgmKind::PATHETIC_HOUSE, nullptr, nullptr},
    {BgmKind::SPOOKTUNE, nullptr, nullptr},
    {BgmKind::SPOOKWAVE, nullptr, nullptr},
    {BgmKind::GHOULIDAY, nullptr, nullptr},
    {BgmKind::CHILL, nullptr, nullptr},
    {BgmKind::THUNDERSNAIL, nullptr, nullptr},
    {BgmKind::TEMMIE_VILLAGE, nullptr, nullptr},
    {BgmKind::TEM_SHOP, nullptr, nullptr},
    {BgmKind::NGAHHH, nullptr, nullptr},
    {BgmKind::SPEAR_OF_JUSTICE, nullptr, nullptr},
    {BgmKind::OOO, nullptr, nullptr},
    {BgmKind::ALPHYS, nullptr, nullptr},
    {BgmKind::ITS_SHOWTIME, nullptr, nullptr},
    {BgmKind::METAL_CRUSHER, nullptr, nullptr},
    {BgmKind::ANOTHER_MEDIUM, nullptr, nullptr},
    {BgmKind::UWA_SO_HEATS, nullptr, nullptr},
    {BgmKind::STRONGER_MONSTERS, nullptr, nullptr},
    {BgmKind::HOTEL, nullptr, nullptr},
    {BgmKind::CAN_YOU_REALLY_CALL_THIS_A_HOTEL_I_DIDNT_RECEIVE_A_MINT_ON_MY_PILLOW_OR_ANYTHING, nullptr, nullptr},
    {BgmKind::CONFESSION, nullptr, nullptr},
    {BgmKind::LIVE_REPORT, nullptr, nullptr},
    {BgmKind::DEATH_REPORT, nullptr, nullptr},
    {BgmKind::SPIDER_DANCE, nullptr, nullptr},
    {BgmKind::WRONG_ENEMY, nullptr, nullptr},
    {BgmKind::OH_ONE_TRUE_LOVE, nullptr, nullptr},
    {BgmKind::OH_DUNGEON, nullptr, nullptr},
    {BgmKind::ITS_RAINING_SOMEWHERE_ELSE, nullptr, nullptr},
    {BgmKind::CORE_APPROACH, nullptr, nullptr},
    {BgmKind::CORE, nullptr, nullptr},
    {BgmKind::LAST_EPISODE, nullptr, nullptr},
    {BgmKind::OH_MY, nullptr, nullptr},
    {BgmKind::DEATH_BY_GLAMOUR, nullptr, nullptr},
    {BgmKind::FOR_THE_FANS, nullptr, nullptr},
    {BgmKind::LONG_ELEVATOR, nullptr, nullptr},
    {BgmKind::UNDERTALE, nullptr, nullptr},
    {BgmKind::SONG_THAT_MIGHT_PLAY_WHEN_YOU_FIGHT_SANS, nullptr, nullptr},
    {BgmKind::THE_CHOICE, nullptr, nullptr},
    {BgmKind::SMALL_SHOCK, nullptr, nullptr},
    {BgmKind::BARRIER, nullptr, nullptr},
    {BgmKind::BERGENTRÜCKUNG, nullptr, nullptr},
    {BgmKind::ASGORE, nullptr, nullptr},
    {BgmKind::YOU_IDIOT, nullptr, nullptr},
    {BgmKind::YOUR_BEST_NIGHTMARE, nullptr, nullptr},
    {BgmKind::FINALE, nullptr, nullptr},
    {BgmKind::AN_ENDING, nullptr, nullptr},
    {BgmKind::SHES_PLAYING_PIANO, nullptr, nullptr},
    {BgmKind::HERE_WE_ARE, nullptr, nullptr},
    {BgmKind::AMALGAM, nullptr, nullptr},
    {BgmKind::FALLEN_DOWN_REPRISE, nullptr, nullptr},
    {BgmKind::DONT_GIVE_UP, nullptr, nullptr},
    {BgmKind::HOPES_AND_DREAMS, nullptr, nullptr},
    {BgmKind::BURN_IN_DESPAIR, nullptr, nullptr},
    {BgmKind::SAVE_THE_WORLD, nullptr, nullptr},
    {BgmKind::HIS_THEME, nullptr, nullptr},
    {BgmKind::FINAL_POWER, nullptr, nullptr},
    {BgmKind::REUNITED, nullptr, nullptr},
    {BgmKind::MENU_FULL, nullptr, nullptr},
    {BgmKind::RESPITE, nullptr, nullptr},
    {BgmKind::BRING_IT_IN_GUYS, nullptr, nullptr},
    {BgmKind::LAST_GOODBYE, nullptr, nullptr},
    {BgmKind::BUT_THE_EARTH_REFUSED_TO_DIE, nullptr, nullptr},
    {BgmKind::BATTLE_AGAINST_A_TRUE_HERO, nullptr, nullptr},
    {BgmKind::POWER_OF_NEO, nullptr, nullptr},
    {BgmKind::MEGALOVANIA, nullptr, nullptr},
    {BgmKind::GOOD_NIGHT, nullptr, nullptr},
};

static_assert(
    []() -> bool {
        for (const auto& bgm : BGMS)
            if (bgm.directMusic && bgm.dmgMusic)
                return false;
        return true;
    }(),
    "Sync direct & dmg music not supported");

auto Bgm::get(BgmKind kind) -> const Bgm&
{
    BN_ASSERT((int)kind < (int)BgmKind::TOTAL_COUNT, "Invalid BgmKind=", (int)kind);

    return BGMS[(int)kind];
}

struct BgmStash
{
    BgmKind bgmKind;
    int directMusPos;
    bn::dmg_music_position dmgPos;
};

static BgmKind gs_playingBgm = BgmKind::NONE;
static bn::optional<BgmStash> gs_stashedBgm = bn::nullopt;
static bn::optional<BgmVolumeToAction> gs_bgmFadeAction;

void Bgm::update()
{
    if (gs_bgmFadeAction && !gs_bgmFadeAction->done())
    {
        gs_bgmFadeAction->update();

        if (gs_bgmFadeAction->done())
        {
            // Fade-out: stash BGM
            if (gs_bgmFadeAction->finalVolume() <= 0.01f)
            {
                pause();
                stash();
            }

            gs_bgmFadeAction = bn::nullopt;
        }
    }
}

bool Bgm::hasStashed()
{
    return gs_stashedBgm.has_value();
}

void Bgm::stash()
{
    BN_ASSERT(isPlaying());
    BN_ASSERT(isPaused());

    gs_stashedBgm =
        BgmStash{.bgmKind = gs_playingBgm,
                 .directMusPos = bn::music::playing() ? bn::music::position() : 0,
                 .dmgPos = bn::dmg_music::playing() ? bn::dmg_music::position() : bn::dmg_music_position(0, 0)};
    stop();
}

void Bgm::restore()
{
    BN_ASSERT(hasStashed());
    BN_ASSERT(!isPlaying());

    const auto& bgm = Bgm::get(gs_stashedBgm->bgmKind);
    if (bgm.directMusic && bgm.dmgMusic)
        BN_ERROR("Sync direct & dmg music not supported"); // bn::audio::set_dmg_sync_enabled(true);

    play(bgm);
    if (bn::music::playing())
        bn::music::set_position(gs_stashedBgm->directMusPos);
    if (bn::dmg_music::playing()) // if you want to sync, row must be `0` instead of `dmgPos.row()`
        bn::dmg_music::set_position(gs_stashedBgm->dmgPos.pattern(), gs_stashedBgm->dmgPos.row());
    pause();

    deleteStashed();
}

void Bgm::deleteStashed()
{
    gs_stashedBgm.reset();
}

auto Bgm::getStashedBgmKind() -> BgmKind
{
    if (hasStashed())
        return gs_stashedBgm->bgmKind;

    return BgmKind::NONE;
}

void Bgm::fadeOutAndStash(int fadeFrames)
{
    BN_ASSERT(isPlaying());

    gs_bgmFadeAction = BgmVolumeToAction(fadeFrames, 0);
}

void Bgm::restoreAndFadeIn(int fadeFrames)
{
    restore();
    resume();

    setVolume(0);
    gs_bgmFadeAction = BgmVolumeToAction(fadeFrames, 1);
}

bool Bgm::isPlaying()
{
    return gs_playingBgm != BgmKind::NONE;
}

auto Bgm::getPlayingBgmKind() -> BgmKind
{
    return gs_playingBgm;
}

void Bgm::play(BgmKind kind)
{
    const Bgm& bgm = Bgm::get(kind);
    play(bgm);
}

void Bgm::play(const Bgm& bgm)
{
    stop();

    if (bgm.directMusic || bgm.dmgMusic)
    {
        if (bgm.directMusic && bgm.dmgMusic)
            BN_ERROR("Sync direct & dmg music not supported"); // bn::audio::set_dmg_sync_enabled(true);

        if (bgm.directMusic)
            bn::music::play(*bgm.directMusic, bgm.directVolume);
        if (bgm.dmgMusic)
            bn::dmg_music::play(*bgm.dmgMusic);

        gs_playingBgm = bgm.kind;
    }
}

void Bgm::stop()
{
    if (gs_playingBgm != BgmKind::NONE)
    {
        const Bgm& bgm = Bgm::get(gs_playingBgm);

        if (bgm.directMusic && bgm.dmgMusic)
            BN_ERROR("Sync direct & dmg music not supported"); // bn::audio::set_dmg_sync_enabled(false);

        if (bgm.directMusic)
            bn::music::stop();
        if (bgm.dmgMusic)
            bn::dmg_music::stop();
    }
    gs_playingBgm = BgmKind::NONE;
}

bool Bgm::isPaused()
{
    if (gs_playingBgm == BgmKind::NONE)
        return false;

    const Bgm& bgm = Bgm::get(gs_playingBgm);
    if (bgm.directMusic)
        return bn::music::paused();
    else if (bgm.dmgMusic)
        return bn::dmg_music::paused();

    BN_ERROR("Should not reach here");
    return false;
}

void Bgm::pause()
{
    BN_ASSERT(isPlaying());
    BN_ASSERT(!isPaused());

    const Bgm& bgm = Bgm::get(gs_playingBgm);
    if (bgm.directMusic)
        bn::music::pause();
    if (bgm.dmgMusic)
        bn::dmg_music::pause();
}

void Bgm::resume()
{
    BN_ASSERT(isPaused());

    const Bgm& bgm = Bgm::get(gs_playingBgm);
    if (bgm.directMusic)
        bn::music::resume();
    if (bgm.dmgMusic)
        bn::dmg_music::resume();
}

auto Bgm::getVolume() -> bn::fixed
{
    BN_ASSERT(isPlaying());

    const Bgm& bgm = Bgm::get(gs_playingBgm);
    if (bgm.dmgMusic)
        return bn::dmg_music::left_volume();
    else if (bgm.directMusic)
        return bn::music::volume() / bgm.directVolume;

    BN_ERROR("Should not reach here");
    return 0;
}

void Bgm::setVolume(bn::fixed volume)
{
    BN_ASSERT(isPlaying());

    const Bgm& bgm = Bgm::get(gs_playingBgm);
    if (bgm.directMusic)
        bn::music::set_volume(volume * bgm.directVolume);
    if (bgm.dmgMusic)
        bn::dmg_music::set_volume(volume);
}

} // namespace ut::asset
