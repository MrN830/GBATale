#pragma once

#include <bn_fixed.h>

#include "asset/BgmKind.hpp"

namespace bn
{
class music_item;
class dmg_music_item;
} // namespace bn

namespace ut::asset
{

/**
 * @brief DMG + Direct Sound music wrapper.
 * Manages `BgmKind` too.
 */
struct Bgm
{
public:
    static auto get(BgmKind) -> const Bgm&;

public:
    BgmKind kind;
    const bn::music_item* directMusic = nullptr;
    const bn::dmg_music_item* dmgMusic = nullptr;
    const bn::fixed directVolume = 1.0 / 8;

public:
    /**
     * @brief Updates bgm system.
     * This must be called every frame.
     */
    static void update();

    static bool hasStashed();
    static void stash();

    /**
     * @brief Restores the BGM, and pause it.
     * You have to call `resume()` manually.
     */
    static void restore();
    static void deleteStashed();
    static auto getStashedBgmKind() -> BgmKind;

    static void fadeOutAndStash(int fadeFrames);
    static void restoreAndFadeIn(int fadeFrames);

    static bool isPlaying();
    static auto getPlayingBgmKind() -> BgmKind;
    static void play(BgmKind);
    static void play(const Bgm&);
    static void stop();

    static bool isPaused();
    static void pause();
    static void resume();

    static auto getVolume() -> bn::fixed;
    static void setVolume(bn::fixed volume);
};

} // namespace ut::asset
