#include "game/cpnt/NpcInput.hpp"

#include "asset/Path.hpp"
#include "game/GameContext.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/TaskManager.hpp"
#include "util/Math.hpp"

namespace ut::game::cpnt
{

NpcInput::NpcInput(ent::Entity& entity, bool isEnabled) : Input(entity, bn::type_id<NpcInput>(), isEnabled)
{
}

void NpcInput::handleInput(GameContext& ctx)
{
    if (isDone() || isPaused())
        sendMoveCmd(cmd::MoveCmd(), ctx);
    else
    {
        switch (_mode)
        {
        case Mode::ONE_WAY:
            handleOneWay(ctx);
            break;
        case Mode::PATH:
            handlePath(ctx);
            break;
        default:
            BN_ERROR("Handling invalid input mode: ", (int)_mode);
        }
    }
}

void NpcInput::startOneWay(const cmd::MoveCmd& cmd, int lastTicks)
{
    BN_ASSERT(cmd.directions != core::Directions::NONE);
    BN_ASSERT(cmd.movePos != bn::fixed_point(0, 0));
    BN_ASSERT(lastTicks > 0, "Invalid ticks: ", lastTicks);

    _mode = Mode::ONE_WAY;
    _isPaused = false;
    _oneWay.moveCmd = cmd;
    _oneWay.remainTicks = lastTicks;
}

void NpcInput::startPath(const asset::IPath& path, bn::fixed speed)
{
    BN_ASSERT(speed > 0, "Non-positive speed: ", speed);
    BN_ASSERT(path.getPoints().size() >= 2, "Path doesn't have 2 points: ", path.getPoints().size());

    _mode = Mode::PATH;
    _isPaused = false;
    _path.ptr = &path;
    _path.pathPointIdx = 1;
    _path.speed = speed;

    const auto startPos = path.getPoints()[0];
    _entity.setPosition(startPos);
}

void NpcInput::setPathSpeed(bn::fixed speed)
{
    BN_ASSERT(_mode == Mode::PATH);
    BN_ASSERT(speed > 0, "Non-position speed: ", speed);

    _path.speed = speed;
}

void NpcInput::pause()
{
    _isPaused = true;
}

void NpcInput::resume()
{
    _isPaused = false;
}

void NpcInput::stop(GameContext& ctx)
{
    if (isDone())
        return;

    _mode = Mode::NONE;

    ctx.taskMngr.onSignal({task::TaskSignal::Kind::NPC_WALK_END, (int)_entity.getId()});
}

bool NpcInput::isPaused() const
{
    return _isPaused;
}

bool NpcInput::isDone() const
{
    return _mode == Mode::NONE;
}

void NpcInput::handleOneWay(GameContext& ctx)
{
    BN_ASSERT(_mode == Mode::ONE_WAY);

    sendMoveCmd(_oneWay.moveCmd, ctx);
    _oneWay.remainTicks -= 1;

    if (_oneWay.remainTicks <= 0)
        stop(ctx);
}

void NpcInput::handlePath(GameContext& ctx)
{
    BN_ASSERT(_mode == Mode::PATH);
    BN_ASSERT(_path.pathPointIdx < _path.ptr->getPoints().size(), "Invalid pathPointIdx: ", _path.pathPointIdx,
              " (max ", _path.ptr->getPoints().size() - 1, ")");

    const auto destPathPoint = _path.ptr->getPoints()[_path.pathPointIdx];

    cmd::MoveCmd moveCmd;
    moveCmd.movePos = util::normalized(destPathPoint - _entity.getPosition());
    moveCmd.movePos *= _path.speed;
    moveCmd.directions = core::pos2Dirs(moveCmd.movePos);
    moveCmd.checkCollision = false;

    sendMoveCmd(moveCmd, ctx);

    // if overshoot
    if (moveCmd.directions != core::pos2Dirs(destPathPoint - _entity.getPosition()))
    {
        // move to `destPathPoint`
        _entity.setPosition(destPathPoint);

        // set next path point, or stop if it was the last path point
        if (++_path.pathPointIdx >= _path.ptr->getPoints().size())
            stop(ctx);
    }
}

} // namespace ut::game::cpnt
