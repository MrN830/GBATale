#pragma once

#include <bn_fixed_point.h>

#include "asset/Path.hpp"
#include "core/Directions.hpp"
#include "util/Math.hpp"

namespace ut::action
{

/**
 * @brief Butano-style path looper.
 *
 * It goes back to initial point of path when it's on last point.
 * This is similar to "closed path" in Game Maker.
 *
 * Unlike Butano's value_template_actions, `Value` is referenced,
 * so it should outlive the `PathLoopValueTemplateAction` to avoid dangling reference.
 *
 * @tparam Value Value to modify.
 * @tparam PositionManager Reads and writes the position of the value to modify.
 */
template <typename Value, typename PositionManager>
class PathLoopValueTemplateAction
{
public:
    void reset()
    {
        _curPos = getAbsPathPoint(0);
        PositionManager::set(_curPos, _value);

        _destPathPointIdx = 1;
    }

    void update()
    {
        bn::fixed remainSpeed = _speed;

        constexpr int LOOP_MAX = 10;
        for (int loop = 0; remainSpeed > 0 && loop < LOOP_MAX; ++loop)
        {
            const auto destPathPoint = getAbsPathPoint(_destPathPointIdx);

            auto moveDiff = util::normalized(destPathPoint - _curPos);
            moveDiff *= remainSpeed;
            auto moveDir = core::pos2Dirs(moveDiff);

            _curPos += moveDiff;

            remainSpeed = 0;

            // if overshoot
            if (moveDir != core::pos2Dirs(destPathPoint - _curPos))
            {
                // move to `destPathPoint`
                _curPos = destPathPoint;

                remainSpeed = util::magnitude(destPathPoint - _curPos);

                // set next path point
                _destPathPointIdx = (_destPathPointIdx + 1) % _path.getPoints().size();
            }
        }

        PositionManager::set(_curPos, _value);
    }

public:
    PathLoopValueTemplateAction(Value& value, bool isAbsolute, const asset::IPath& path, bn::fixed speed)
        : _value(value), _anchorPos(isAbsolute ? bn::fixed_point(0, 0) : PositionManager::get(value)), _path(path),
          _speed(speed)
    {
        BN_ASSERT(speed > 0, "Non-positive speed: ", speed);
        BN_ASSERT(path.getPoints().size() >= 2, "Path doesn't have 2 points: ", path.getPoints().size());

        reset();
    }

private:
    auto getAbsPathPoint(int pathPointIdx) -> bn::fixed_point
    {
        auto pathPoints = _path.getPoints();

        BN_ASSERT(0 <= pathPointIdx && pathPointIdx < pathPoints.size(), "pathPointIdx=", pathPointIdx, " OOB (max ",
                  pathPoints.size() - 1, ")");

        return _anchorPos + pathPoints[pathPointIdx];
    }

private:
    Value& _value;

    const bn::fixed_point _anchorPos;
    const asset::IPath& _path;
    const bn::fixed _speed;

    bn::fixed_point _curPos;
    int _destPathPointIdx;
};

} // namespace ut::action
