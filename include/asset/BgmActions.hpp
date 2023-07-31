#pragma once

#include "asset/Bgm.hpp"

#include <bn_template_actions.h>

namespace ut::asset
{

class BgmVolumeManager
{
public:
    static bn::fixed get();
    static void set(bn::fixed volume);
};

class BgmVolumeToAction : public bn::to_template_action<bn::fixed, BgmVolumeManager>
{
public:
    BgmVolumeToAction(int durationUpdates, bn::fixed finalVolume);

    bn::fixed finalVolume() const;
};

} // namespace ut::asset
