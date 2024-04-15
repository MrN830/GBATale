#pragma once

#include "action/ShakeValueTemplateAction.hpp"

#include "action/CameraPositionManager.hpp"

namespace ut::action
{

class CameraShakeAction : public ShakeValueTemplateAction<bn::camera_ptr, CameraPositionManager>
{
public:
    CameraShakeAction(bn::camera_ptr& cam, ShakeStyle shakeStyle, const bn::fixed_size& shakeScale,
                      const bn::fixed_size& shakeDecrease, int durationUpdates)
        : ShakeValueTemplateAction(cam, shakeStyle, shakeScale, shakeDecrease, durationUpdates)
    {
    }
};

} // namespace ut::action
