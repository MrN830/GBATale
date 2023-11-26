#pragma once

#include <bn_camera_ptr.h>
#include <bn_fixed_point.h>

namespace ut::game
{
struct GameContext;
}
namespace ut::game::ent
{
class Entity;
}

namespace ut::game::sys
{

class CameraManager final
{
public:
    CameraManager();

    void update(GameContext&);

public:
    auto getCamFollowEntity() const -> const ent::Entity*;
    void setCamFollowEntity(const ent::Entity*);

    auto getCamFollowEntityDiff() const -> const bn::fixed_point&;
    void setCamFollowEntityDiff(const bn::fixed_point&);

    auto getCamera() -> bn::camera_ptr&;
    auto getCamera() const -> const bn::camera_ptr&;

private:
    void snapCamToRoom(const GameContext&);

private:
    bn::camera_ptr _camera;

    const ent::Entity* _camFollowEntity = nullptr;
    bn::fixed_point _camFollowEntityDiff;
};

} // namespace ut::game::sys
