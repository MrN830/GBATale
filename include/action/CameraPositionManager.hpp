#pragma once

#include <bn_camera_ptr.h>

namespace ut::action
{

class CameraPositionManager
{
public:
    static auto get(const bn::camera_ptr& cam) -> const bn::fixed_point&
    {
        return cam.position();
    }

    static void set(const bn::fixed_point& pos, bn::camera_ptr& cam)
    {
        cam.set_position(pos);
    }
};

} // namespace ut::action
