#pragma once

namespace bn
{
class sound_item;
}

namespace ut::asset
{

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
    ITEM,
    SWITCH_TRIGGER,
    PHONE,
    SAVE,

    MENU_CURSOR,
    MENU_ACTIVATE,

    VOICE_INTRO,
    VOICE_TORIEL,
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

auto getSfx(SfxKind) -> const bn::sound_item*;

} // namespace ut::asset
