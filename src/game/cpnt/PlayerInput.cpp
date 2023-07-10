#include "game/cpnt/PlayerInput.hpp"

#include <bn_keypad.h>

#include "asset/SpriteAnimKind.hpp"
#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/cmd/InputCmd.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/RoomChanger.hpp"
#include "mtile/MTilemap.hpp"

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
        {
            cmd.directions |= core::Directions::UP;
            cmd.movePos.set_y(cmd.movePos.y() - PLAYER_SPEED);
        }
        else if (bn::keypad::down_held())
        {
            cmd.directions |= core::Directions::DOWN;
            cmd.movePos.set_y(cmd.movePos.y() + PLAYER_SPEED);
        }
        if (bn::keypad::left_held())
        {
            cmd.directions |= core::Directions::LEFT;
            cmd.movePos.set_x(cmd.movePos.x() - PLAYER_SPEED);
        }
        else if (bn::keypad::right_held())
        {
            cmd.directions |= core::Directions::RIGHT;
            cmd.movePos.set_x(cmd.movePos.x() + PLAYER_SPEED);
        }
    }

    cmd = handleWarpCollision(cmd, ctx);

    sendInput(cmd, ctx);
}

auto PlayerInput::handleWarpCollision(const cmd::InputCmd& cmd, GameContext& ctx) -> cmd::InputCmd
{
    auto result = cmd;

    _entity.setPosition(_entity.getPosition() + cmd.movePos);

    const auto* collPack = _entity.getComponent<ColliderPack>();
    BN_ASSERT(collPack != nullptr);

    const auto* mTilemap = getRoomMTilemap(ctx.state.getRoom());
    BN_ASSERT(mTilemap != nullptr);

    for (const auto& warp : mTilemap->getWarps())
    {
        if (collPack->isCollideWith(warp.collInfo))
        {
            ctx.roomChanger.reqChange(warp.room, warp.warpId, ctx);
            // Prevent animation direction change on diagonal wall collision
            result.movePos = {0, 0};
            break;
        }
    }

    _entity.setPosition(_entity.getPosition() - cmd.movePos);

    return result;
}

} // namespace ut::game::cpnt
