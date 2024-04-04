#include "game/sys/CameraManager.hpp"

#include <bn_display.h>
#include <bn_fixed_point.h>

#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/TaskManager.hpp"
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
    if (_camFollowEntity && !hasShake())
    {
        _camera.set_position(_camFollowEntity->getPosition() + _camFollowEntityDiff);
        snapCamToRoom(ctx);
        ctx.isDialogUpper =
            (_camFollowEntity->getPosition().y() - (_camera.position().y() - _camFollowEntityDiff.y()) >=
             DIALOG_UPPER_Y_DIFF);
    }

    if (hasShake())
        updateShake(ctx);
}

void CameraManager::startShake(const bn::fixed_size& shakeScale, const bn::fixed_size& shakeDecrease,
                               const int durationUpdate)
{
    _camShake.emplace(_camera, core::ShakeStyle::ONE_WAY, shakeScale, shakeDecrease, durationUpdate);
}

auto CameraManager::getCamFollowEntity() const -> const ent::Entity*
{
    return _camFollowEntity;
}

void CameraManager::setCamFollowEntity(const ent::Entity* entity)
{
    _camFollowEntity = entity;
}

auto CameraManager::getCamFollowEntityDiff() const -> const bn::fixed_point&
{
    return _camFollowEntityDiff;
}

void CameraManager::setCamFollowEntityDiff(const bn::fixed_point& diff)
{
    _camFollowEntityDiff = diff;
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

    BN_ASSERT(mTilemap != nullptr);

    const bn::fixed_point roomBottomRight = {mTilemap->getWidth() * 16 - INIT_CAM_POS.x(),
                                             mTilemap->getHeight() * 16 - INIT_CAM_POS.y()};

    if (mTilemap->getWidth() <= bn::display::width() / 16)
        _camera.set_x(mTilemap->getWidth() * 16 / 2);
    else if (_camera.x() < INIT_CAM_POS.x())
        _camera.set_x(INIT_CAM_POS.x());
    else if (_camera.x() > roomBottomRight.x())
        _camera.set_x(roomBottomRight.x());

    if (mTilemap->getHeight() <= bn::display::height() / 16)
        _camera.set_y(mTilemap->getHeight() * 16 / 2);
    else if (_camera.y() < INIT_CAM_POS.y())
        _camera.set_y(INIT_CAM_POS.y());
    else if (_camera.y() > roomBottomRight.y())
        _camera.set_y(roomBottomRight.y());
}

bool CameraManager::hasShake() const
{
    return _camShake.has_value();
}

void CameraManager::updateShake(GameContext& ctx)
{
    if (!hasShake())
        return;

    _camShake->update();

    if (_camShake->isDone())
    {
        _camShake.reset();
        ctx.taskMngr.onSignal({task::TaskSignal::Kind::CAM_SHAKE_END});
    }
}

} // namespace ut::game::sys
