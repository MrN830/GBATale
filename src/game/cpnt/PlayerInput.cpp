#include "game/cpnt/PlayerInput.hpp"

#include <bn_keypad.h>

#include "game/GameContext.hpp"
#include "game/cmd/InputCmd.hpp"

namespace ut::game::cpnt
{

PlayerInput::PlayerInput(ent::Entity& entity) : Input(entity)
{
}

auto PlayerInput::getType() const -> bn::type_id_t
{
    return bn::type_id<PlayerInput>();
}

void PlayerInput::handleInput(GameContext& ctx)
{
    constexpr bn::fixed PLAYER_SPEED = 3;

    cmd::InputCmd cmd;

    if (!ctx.isShowingUI)
    {
        if (bn::keypad::up_held())
            cmd.movePos.set_y(cmd.movePos.y() - PLAYER_SPEED);
        else if (bn::keypad::down_held())
            cmd.movePos.set_y(cmd.movePos.y() + PLAYER_SPEED);
        if (bn::keypad::left_held())
            cmd.movePos.set_x(cmd.movePos.x() - PLAYER_SPEED);
        else if (bn::keypad::right_held())
            cmd.movePos.set_x(cmd.movePos.x() + PLAYER_SPEED);
    }

    sendInput(fixMoveCmdCollision(cmd));
}

} // namespace ut::game::cpnt
