#pragma once

#include <bn_camera_ptr.h>

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

    auto getCamera() -> bn::camera_ptr&;
    auto getCamera() const -> const bn::camera_ptr&;

private:
    void snapCamToRoom(const GameContext&);

private:
    bn::camera_ptr _camera;

    const ent::Entity* _camFollowEntity = nullptr;
};

} // namespace ut::game::sys
