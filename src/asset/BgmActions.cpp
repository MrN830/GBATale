#include "asset/BgmActions.hpp"

#include <bn_assert.h>

namespace ut::asset
{

bn::fixed BgmVolumeManager::get()
{
    return Bgm::getVolume();
}

void BgmVolumeManager::set(bn::fixed volume)
{
    Bgm::setVolume(volume);
}

BgmVolumeToAction::BgmVolumeToAction(int durationUpdates, bn::fixed finalVolume)
    : to_template_action(durationUpdates, finalVolume)
{
    BN_ASSERT(0 <= finalVolume && finalVolume <= 1, "Invalid final volume: ", finalVolume);
}

bn::fixed BgmVolumeToAction::finalVolume() const
{
    return final_property();
}

} // namespace ut::asset
