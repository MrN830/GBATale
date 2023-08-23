#include "game/cpnt/NpcInput.hpp"

#include "game/GameContext.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/TaskManager.hpp"

namespace ut::game::cpnt
{

NpcInput::NpcInput(ent::Entity& entity, bool isEnabled) : Input(entity, bn::type_id<NpcInput>(), isEnabled)
{
}

void NpcInput::handleInput(GameContext& ctx)
{
    if (isDone())
        sendMoveCmd(cmd::MoveCmd(), ctx);
    else
    {
        sendMoveCmd(_moveCmd, ctx);
        _remainTicks -= 1;

        if (isDone())
            ctx.taskMngr.onSignal({task::TaskSignal::Kind::NPC_WALK_END, (int)_entity.getId()});
    }
}

void NpcInput::startInput(const cmd::MoveCmd& cmd, int lastTicks)
{
    BN_ASSERT(cmd.directions != core::Directions::NONE);
    BN_ASSERT(cmd.movePos != bn::fixed_point(0, 0));
    BN_ASSERT(lastTicks > 0, "Invalid ticks: ", lastTicks);

    _moveCmd = cmd;
    _remainTicks = lastTicks;
}

void NpcInput::stopInput()
{
    _moveCmd = cmd::MoveCmd();
    _remainTicks = 0;
}

bool NpcInput::isDone() const
{
    return _remainTicks <= 0 || _moveCmd.directions == core::Directions::NONE;
}

} // namespace ut::game::cpnt
