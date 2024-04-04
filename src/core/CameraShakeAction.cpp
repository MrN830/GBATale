#include "core/CameraShakeAction.hpp"

namespace ut::core
{

auto CameraShakePosManager::get(const bn::camera_ptr& cam) -> const bn::fixed_point&
{
    return cam.position();
}

void CameraShakePosManager::set(const bn::fixed_point& pos, bn::camera_ptr& cam)
{
    cam.set_position(pos);
}

CameraShakeAction::CameraShakeAction(bn::camera_ptr& cam, core::ShakeStyle shakeStyle, const bn::fixed_size& shakeScale,
                                     const bn::fixed_size& shakeDecrease, int durationUpdates)
    : ShakeValueTemplateAction(cam, shakeStyle, shakeScale, shakeDecrease, durationUpdates)
{
}

} // namespace ut::core
