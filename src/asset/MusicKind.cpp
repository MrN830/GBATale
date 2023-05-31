#include "asset/MusicKind.hpp"

#include <bn_assert.h>

#include "bn_music_items.h"

namespace ut::asset
{

static constexpr const bn::music_item* MUSICS[(int)MusicKind::TOTAL_COUNT] = {
    &bn::music_items::once_upon_a_time,
    &bn::music_items::menu,
    &bn::music_items::fallen_down,
};

auto getMusic(MusicKind kind) -> const bn::music_item*
{
    BN_ASSERT((int)MusicKind::NONE <= (int)kind && (int)kind < (int)MusicKind::TOTAL_COUNT);

    if (kind == MusicKind::NONE)
        return nullptr;

    return MUSICS[(int)kind];
}

} // namespace ut::asset
