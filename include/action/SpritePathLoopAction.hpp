#pragma once

#include "action/PathLoopValueTemplateAction.hpp"

#include "action/SpritePositionManager.hpp"

namespace ut::action
{

class SpritePathLoopAction : public PathLoopValueTemplateAction<bn::sprite_ptr, SpritePositionManager>
{
public:
    SpritePathLoopAction(bn::sprite_ptr& spr, bool isAbsolute, const asset::IPath& path, bn::fixed speed)
        : PathLoopValueTemplateAction(spr, isAbsolute, path, speed)
    {
    }
};

} // namespace ut::action
