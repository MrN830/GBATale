#include "asset/SfxKind.hpp"

#include <bn_assert.h>
#include <bn_vector.h>

#include "bn_sound_items.h"

namespace ut::asset
{

static bn::vector<bn::pair<SfxKind, int>, 4> g_delayedSfxs;

static constexpr const bn::sound_item* SFXS[(int)SfxKind::TOTAL_COUNT] = {
    &bn::sound_items::mus_intronoise,
    &bn::sound_items::mus_cymbal,

    &bn::sound_items::snd_b,
    &bn::sound_items::snd_battlefall,
    &bn::sound_items::snd_heal_c,
    &bn::sound_items::snd_power,
    &bn::sound_items::snd_hurt1,
    &bn::sound_items::snd_hurt1_c,
    &bn::sound_items::snd_swallow,
    &bn::sound_items::snd_item,
    &bn::sound_items::snd_noise,
    &bn::sound_items::snd_switchpull_n,
    &bn::sound_items::snd_screenshake,
    &bn::sound_items::snd_fall2,
    &bn::sound_items::snd_phone,
    &bn::sound_items::snd_save,

    &bn::sound_items::snd_speedup,
    &bn::sound_items::snd_spooky,
    &bn::sound_items::snd_sparkle1,
    &bn::sound_items::snd_pombark,
    &bn::sound_items::snd_dogresidue,
    &bn::sound_items::snd_dogsalad,
    &bn::sound_items::snd_catsalad,
    &bn::sound_items::snd_movemenu,
    &bn::sound_items::snd_hero,
    &bn::sound_items::snd_wrongvictory,

    &bn::sound_items::snd_squeak,
    &bn::sound_items::snd_select,

    &bn::sound_items::snd_txt2,
    &bn::sound_items::snd_txttor,
    &bn::sound_items::snd_txttor2,
    &bn::sound_items::snd_txt1, // TODO: change sfx to SANS_TORIEL
    &bn::sound_items::snd_txt1,
    &bn::sound_items::snd_txt1, // TODO: change sfx to DEFAULT_NO_SOUND
    &bn::sound_items::snd_txt1, // TODO: change sfx to FLOWEY_EVIL
    &bn::sound_items::snd_txt1, // TODO: change sfx to SANS
    &bn::sound_items::snd_txt1, // TODO: change sfx to PAPYRUS
    &bn::sound_items::snd_txt1, // TODO: change sfx to METTATON
    &bn::sound_items::snd_txt1, // TODO: change sfx to UNDYNE
    &bn::sound_items::snd_txt1, // TODO: change sfx to ALPHYS
    &bn::sound_items::snd_txt1, // TODO: change sfx to ASGORE
    &bn::sound_items::snd_txt1, // TODO: change sfx to ASRIEL
};

auto getSfx(SfxKind kind) -> const bn::sound_item*
{
    BN_ASSERT((int)SfxKind::NONE <= (int)kind && (int)kind < (int)SfxKind::TOTAL_COUNT);

    if (kind == SfxKind::NONE)
        return nullptr;

    return SFXS[(int)kind];
}

void playSfxWithDelay(SfxKind sfx, int delay)
{
    BN_ASSERT(!g_delayedSfxs.full(), "too many reserved delayed SFXs (max ", g_delayedSfxs.max_size(), ")");

    if (sfx != SfxKind::NONE && getSfx(sfx) != nullptr)
        g_delayedSfxs.emplace_back(sfx, delay);
}

void updateDelayedSfxs()
{
    for (auto it = g_delayedSfxs.begin(); it != g_delayedSfxs.end();)
    {
        if (it->second-- <= 0)
        {
            getSfx(it->first)->play();
            it = g_delayedSfxs.erase(it);
        }
        else
            ++it;
    }
}

} // namespace ut::asset
