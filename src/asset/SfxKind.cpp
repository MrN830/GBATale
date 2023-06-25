#include "asset/SfxKind.hpp"

#include <bn_assert.h>

#include "bn_sound_items.h"

namespace ut::asset
{

static constexpr const bn::sound_item* SFXS[(int)SfxKind::TOTAL_COUNT] = {
    &bn::sound_items::mus_intronoise, &bn::sound_items::mus_cymbal,

    &bn::sound_items::snd_b,          &bn::sound_items::snd_battlefall, &bn::sound_items::snd_heal_c,
    &bn::sound_items::snd_power,      &bn::sound_items::snd_hurt1,      &bn::sound_items::snd_hurt1_c, &bn::sound_items::snd_swallow,
    &bn::sound_items::snd_item,       &bn::sound_items::snd_noise,      &bn::sound_items::snd_phone,
    &bn::sound_items::snd_save,

    &bn::sound_items::snd_squeak,     &bn::sound_items::snd_select,

    &bn::sound_items::snd_txt2,
    &bn::sound_items::snd_txt2, // TODO: change sfx to TORIEL
    &bn::sound_items::snd_txt2, // TODO: change sfx to SANS_TORIEL
    &bn::sound_items::snd_txt2, // TODO: change sfx to DEFAULT
    &bn::sound_items::snd_txt2, // TODO: change sfx to DEFAULT_NO_SOUND
    &bn::sound_items::snd_txt2, // TODO: change sfx to FLOWEY_EVIL
    &bn::sound_items::snd_txt2, // TODO: change sfx to SANS
    &bn::sound_items::snd_txt2, // TODO: change sfx to PAPYRUS
    &bn::sound_items::snd_txt2, // TODO: change sfx to METTATON
    &bn::sound_items::snd_txt2, // TODO: change sfx to UNDYNE
    &bn::sound_items::snd_txt2, // TODO: change sfx to ALPHYS
    &bn::sound_items::snd_txt2, // TODO: change sfx to ASGORE
    &bn::sound_items::snd_txt2, // TODO: change sfx to ASRIEL
};

auto getSfx(SfxKind kind) -> const bn::sound_item*
{
    BN_ASSERT((int)SfxKind::NONE <= (int)kind && (int)kind < (int)SfxKind::TOTAL_COUNT);

    if (kind == SfxKind::NONE)
        return nullptr;

    return SFXS[(int)kind];
}

} // namespace ut::asset
