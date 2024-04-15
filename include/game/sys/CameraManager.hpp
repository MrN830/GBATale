#pragma once

#include <bn_camera_ptr.h>

#include "action/CameraShakeAction.hpp"

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

    void startShake(const bn::fixed_size& shakeScale, const bn::fixed_size& shakeDecrease, int waitUpdate);

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

    bn::optional<action::CameraShakeAction> _camShake;
};

} // namespace ut::game::sys
