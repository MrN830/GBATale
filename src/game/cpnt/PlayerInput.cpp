#include "game/cpnt/PlayerInput.hpp"

#include <bn_keypad.h>

#include "asset/SpriteAnimKind.hpp"
#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/cmd/MoveCmd.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/cpnt/inter/Interaction.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/RoomChanger.hpp"
#include "mtile/MTilemap.hpp"
#include "scene/Game.hpp"

namespace ut::game::cpnt
{

namespace
{

constexpr coll::RectCollInfo DOWN_INTERACTOR = {bn::fixed_point(-0.5, 3), bn::fixed_size(9, 20)};
constexpr coll::RectCollInfo UP_INTERACTOR = {bn::fixed_point(-0.5, -11.5), bn::fixed_size(9, 13)};
constexpr coll::RectCollInfo LEFT_INTERACTOR = {bn::fixed_point(-10, -4.5), bn::fixed_size(20, 9)};
constexpr coll::RectCollInfo RIGHT_INTERACTOR = {bn::fixed_point(+10, -4.5), bn::fixed_size(20, 9)};

}; // namespace

PlayerInput::PlayerInput(ent::Entity& entity, const WalkAnimCtrl& walk) : Input(entity), _walk(walk)
{
}

auto PlayerInput::getType() const -> bn::type_id_t
{
    return bn::type_id<PlayerInput>();
}

void PlayerInput::handleInput(GameContext& ctx)
{
    constexpr bn::fixed PLAYER_SPEED = 3;

    cmd::MoveCmd cmd;

    if (bn::keypad::start_pressed() || bn::keypad::l_pressed() || bn::keypad::r_pressed())
    {
        // TODO: Disable opening menu on free-to-move cutscenes (e.g. Mettaton's TV show, Undyne's chases)
        ctx.game.openIngameMenu();
    }
    else if (bn::keypad::a_pressed())
    {
        const auto interactColl = getInteractCollInfo();

        auto it = ctx.entMngr.beforeBeginIter();
        while (it != ctx.entMngr.endIter())
        {
            it = ctx.entMngr.findIf(
                [&interactColl](const ent::Entity& entity) -> bool {
                    const auto* interaction = entity.getComponent<cpnt::inter::Interaction>();
                    if (interaction == nullptr || !interaction->isEnabled())
                        return false;
                    if (!(interaction->getTriggers() & inter::InteractionTriggers::PRESS_A))
                        return false;

                    const auto* collPack = entity.getComponent<cpnt::ColliderPack>();
                    if (collPack == nullptr || !collPack->isEnabled())
                        return false;
                    if (!collPack->isCollideWith(interactColl))
                        return false;

                    return true;
                },
                it);

            if (it != ctx.entMngr.endIter())
            {
                auto* interaction = it->getComponent<cpnt::inter::Interaction>();
                BN_ASSERT(interaction != nullptr);

                interaction->onInteract();
            }
        }
    }

    // If non FREE, `sendMoveCmd()` will send NO movement, stopping Frisk walking animation.
    if (ctx.interactState == InteractState::FREE)
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

    sendMoveCmd(cmd, ctx);
}

auto PlayerInput::handleWarpCollision(const cmd::MoveCmd& cmd, GameContext& ctx) -> cmd::MoveCmd
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

auto PlayerInput::getInteractCollInfo() const -> coll::RectCollInfo
{
    using Dirs = core::Directions;

    const auto dir = _walk.getLastAnimDir();
    BN_ASSERT(dir == Dirs::UP || dir == Dirs::DOWN || dir == Dirs::LEFT || dir == Dirs::RIGHT);

    auto coll = (dir == Dirs::UP     ? UP_INTERACTOR
                 : dir == Dirs::DOWN ? DOWN_INTERACTOR
                 : dir == Dirs::LEFT ? LEFT_INTERACTOR
                                     : RIGHT_INTERACTOR);
    coll.position += _entity.getPosition();

    return coll;
}

} // namespace ut::game::cpnt
