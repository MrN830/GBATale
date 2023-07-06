#include "game/sys/CameraManager.hpp"

#include <bn_display.h>
#include <bn_fixed_point.h>

#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/ent/Entity.hpp"
#include "mtile/MTilemap.hpp"

namespace ut::game::sys
{

namespace
{
constexpr bn::fixed_point INIT_CAM_POS = {bn::display::width() / 2, bn::display::height() / 2};
constexpr bn::fixed DIALOG_UPPER_Y_DIFF = 7;
} // namespace

CameraManager::CameraManager() : _camera(bn::camera_ptr::create(INIT_CAM_POS))
{
}

void CameraManager::update(GameContext& ctx)
{
    if (_camFollowEntity)
    {
        auto& camera = ctx.camMngr.getCamera();
        camera.set_position(_camFollowEntity->getPosition());
        snapCamToRoom(ctx);
        ctx.isDialogUpper = (_camFollowEntity->getPosition().y() - camera.position().y() >= DIALOG_UPPER_Y_DIFF);
    }
}

auto CameraManager::getCamFollowEntity() const -> const ent::Entity*
{
    return _camFollowEntity;
}

void CameraManager::setCamFollowEntity(const ent::Entity* entity)
{
    _camFollowEntity = entity;
}

auto CameraManager::getCamera() -> bn::camera_ptr&
{
    return _camera;
}

auto CameraManager::getCamera() const -> const bn::camera_ptr&
{
    return _camera;
}

void CameraManager::snapCamToRoom(const GameContext& ctx)
{
    const auto* mTilemap = getRoomMTilemap(ctx.state.getRoom());
    auto& camera = ctx.camMngr.getCamera();

    BN_ASSERT(mTilemap != nullptr);

    const bn::fixed_point roomBottomRight = {mTilemap->getWidth() * 16 - INIT_CAM_POS.x(),
                                             mTilemap->getHeight() * 16 - INIT_CAM_POS.y()};

    if (mTilemap->getWidth() <= bn::display::width() / 16)
        camera.set_x(mTilemap->getWidth() * 16 / 2);
    else if (camera.x() < INIT_CAM_POS.x())
        camera.set_x(INIT_CAM_POS.x());
    else if (camera.x() > roomBottomRight.x())
        camera.set_x(roomBottomRight.x());

    if (mTilemap->getHeight() <= bn::display::height() / 16)
        camera.set_y(mTilemap->getHeight() * 16 / 2);
    else if (camera.y() < INIT_CAM_POS.y())
        camera.set_y(INIT_CAM_POS.y());
    else if (camera.y() > roomBottomRight.y())
        camera.set_y(roomBottomRight.y());
}

} // namespace ut::game::sys
