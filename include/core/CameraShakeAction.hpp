#pragma once

#include "core/ShakeValueTemplateAction.hpp"

#include <bn_camera_ptr.h>

namespace ut::core
{

class CameraShakePosManager
{
public:
    static auto get(const bn::camera_ptr& cam) -> const bn::fixed_point&;
    static void set(const bn::fixed_point& pos, bn::camera_ptr& cam);
};

class CameraShakeAction : public core::ShakeValueTemplateAction<bn::camera_ptr, CameraShakePosManager>
{
public:
    CameraShakeAction(bn::camera_ptr& cam, core::ShakeStyle shakeStyle, const bn::fixed_size& shakeScale,
                      const bn::fixed_size& shakeDecrease, int durationUpdates);
};

} // namespace ut::core
