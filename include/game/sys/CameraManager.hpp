#pragma once

#include <bn_camera_ptr.h>
#include <bn_fixed_point.h>
#include <bn_fixed_size.h>

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

    void startShake(const bn::fixed_size& shakeScale, int16_t waitUpdate);

public:
    auto getCamFollowEntity() const -> const ent::Entity*;
    void setCamFollowEntity(const ent::Entity*);

    auto getCamFollowEntityDiff() const -> const bn::fixed_point&;
    void setCamFollowEntityDiff(const bn::fixed_point&);

    auto getCamera() -> bn::camera_ptr&;
    auto getCamera() const -> const bn::camera_ptr&;

private:
    void snapCamToRoom(const GameContext&);

    bool hasShake() const;
    void updateShake(GameContext&);

private:
    bn::camera_ptr _camera;

    const ent::Entity* _camFollowEntity = nullptr;
    bn::fixed_point _camFollowEntityDiff;

    bn::fixed_point _shakeScale;
    int16_t _shakeWaitUpdate = -1;
    int16_t _shakeCountdown = -1;
    bool _curShakeDir = true;
};

} // namespace ut::game::sys
