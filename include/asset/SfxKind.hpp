#pragma once

namespace bn
{
class sound_item;
}

namespace ut::asset
{

enum class SfxKind;

auto getSfx(SfxKind) -> const bn::sound_item*;
void playSfxWithDelay(SfxKind, int delay);

enum class SfxKind
{
    NONE = -1,

    INTRO_IMPACT = 0,
    CYMBAL_RISER,

    ENCOUNTER_ALERT,
    BATTLE_INIT,
    HEAL_SMALL,
    HEAL_BIG,
    HURT_SMALL,
    HURT_BIG,
    SWALLOW,
    ITEM,
    SWITCH_TRIGGER,
    SWITCH_PULL_N,
    SCREEN_SHAKE,
    FALL,
    PHONE,
    SAVE,

    SPEED_UP,
    SPOOKY,
    SPARKLE1,
    DOG_RESIDUE,
    DOG_SALAD,
    CAT_SALAD,
    MOUSE_SQUEAK,
    HERO,
    WRONG_VICTORY,

    MENU_CURSOR,
    MENU_ACTIVATE,

    VOICE_INTRO,
    VOICE_TORIEL,
    VOICE_TORIEL_LOWER,
    VOICE_SANS_TORIEL,
    VOICE_DEFAULT,
    VOICE_DEFAULT_NO_SOUND,
    VOICE_FLOWEY_EVIL,
    VOICE_SANS,
    VOICE_PAPYRUS,
    VOICE_METTATON,
    VOICE_UNDYNE,
    VOICE_ALPHYS,
    VOICE_ASGORE,
    VOICE_ASRIEL,

    TOTAL_COUNT
};

} // namespace ut::asset
