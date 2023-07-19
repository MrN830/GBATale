#include "game/cpnt/Input.hpp"

#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/cmd/MoveCmd.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/cpnt/inter/Interaction.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/EntityManager.hpp"
#include "gen/EntityId.hpp"
#include "mtile/MTilemap.hpp"

namespace ut::game::cpnt
{

Input::Input(ent::Entity& entity) : Component(entity)
{
}

void Input::sendMoveCmd(const cmd::MoveCmd& cmd, GameContext& ctx)
{
    collideWithInteraction(cmd, ctx);

    const auto collFixedCmd = (cmd.checkCollision ? fixMoveCmdCollision(cmd, ctx) : cmd);

    // translate entity position
    _entity.setPosition(_entity.getPosition() + collFixedCmd.movePos);

    // send `cmd` to `cpnt::WalkAnimCtrl`
    auto* walkAnimCtrl = _entity.getComponent<cpnt::WalkAnimCtrl>();
    if (walkAnimCtrl != nullptr)
        walkAnimCtrl->receiveMoveCmd(collFixedCmd);
}

void Input::collideWithInteraction(const cmd::MoveCmd& cmd, GameContext& ctx)
{
    const auto* collPack = _entity.getComponent<cpnt::ColliderPack>();
    if (collPack == nullptr)
        return;

    if (cmd.movePos != bn::fixed_point{0, 0})
    {
        _entity.setPosition(_entity.getPosition() + cmd.movePos);

        auto it = ctx.entMngr.beforeBeginIter();
        while (it != ctx.entMngr.endIter())
        {
            it = ctx.entMngr.findIf(
                [this, collPack](const ent::Entity& entity) -> bool {
                    if (&entity == &_entity)
                        return false;

                    const auto* interaction = entity.getComponent<inter::Interaction>();
                    if (interaction == nullptr || !interaction->isEnabled())
                        return false;

                    const auto thisInterTrig =
                        ((_entity.getId() == ent::gen::EntityId::frisk) ? inter::InteractionTriggers::COLLIDE_FRISK
                                                                        : inter::InteractionTriggers::COLLIDE_OTHERS);
                    if (!(interaction->getTriggers() & thisInterTrig))
                        return false;

                    const auto* otherColl = entity.getComponent<ColliderPack>();
                    if (otherColl == nullptr || !otherColl->isEnabled())
                        return false;
                    if (!collPack->isCollideWith(*otherColl))
                        return false;

                    return true;
                },
                it);

            if (it != ctx.entMngr.endIter())
            {
                auto* interaction = it->getComponent<inter::Interaction>();
                BN_ASSERT(interaction != nullptr);

                interaction->onInteract();
            }
        }

        _entity.setPosition(_entity.getPosition() - cmd.movePos);
    }
}

auto Input::fixMoveCmdCollision(const cmd::MoveCmd& cmd, GameContext& ctx) -> cmd::MoveCmd
{
    const auto* collPack = _entity.getComponent<ColliderPack>();
    if (collPack == nullptr || collPack->isTrigger())
        return cmd;

    auto result = cmd;

    const auto* mTilemap = getRoomMTilemap(ctx.state.getRoom());
    BN_ASSERT(mTilemap != nullptr);

    auto hasStopCollision = [this, &ctx, collPack, mTilemap](const sys::EntityManager& entMngr) -> bool {
        // rect wall collision
        for (const auto& wall : mTilemap->getRectWalls())
            if (collPack->isCollideWith(wall))
                return true;

        // other entity collision
        auto collWithOtherEntity = [this, &ctx, collPack](const ent::Entity& entity) -> bool {
            if (&entity == &_entity)
                return false;

            const auto* otherColl = entity.getComponent<ColliderPack>();
            if (otherColl == nullptr || !otherColl->isEnabled())
                return false;
            if (otherColl->isTrigger() || !collPack->isCollideWith(*otherColl))
                return false;

            return true;
        };

        if (entMngr.findIf(collWithOtherEntity, ctx.entMngr.beforeBeginIter()) != ctx.entMngr.endIter())
            return true;

        return false;
    };

    // stop walls
    if (cmd.movePos.x() != 0)
    {
        const bn::fixed_point tempDiff{cmd.movePos.x(), 0};
        _entity.setPosition(_entity.getPosition() + tempDiff);
        if (hasStopCollision(ctx.entMngr))
            result.movePos.set_x(0);
        _entity.setPosition(_entity.getPosition() - tempDiff);
    }
    if (cmd.movePos.y() != 0)
    {
        const bn::fixed_point tempDiff{0, cmd.movePos.y()};
        _entity.setPosition(_entity.getPosition() + tempDiff);
        if (hasStopCollision(ctx.entMngr))
            result.movePos.set_y(0);
        _entity.setPosition(_entity.getPosition() - tempDiff);
    }

    // slide through triangle walls
    const bn::fixed_point tempDiff = result.movePos;
    const auto tempDirs = core::pos2Dirs(tempDiff);

    _entity.setPosition(_entity.getPosition() + tempDiff);

    for (const auto& wall : mTilemap->getTriWalls())
    {
        if (collPack->isCollideWith(wall))
        {
            using Dirs = core::Directions;

            const bool wallUp = !!(wall.directions & core::Directions::UP);
            const bool wallDown = !!(wall.directions & core::Directions::DOWN);
            const bool wallLeft = !!(wall.directions & core::Directions::LEFT);
            const bool wallRight = !!(wall.directions & core::Directions::RIGHT);

            if (!!(tempDirs & Dirs::UP) && wallUp)
                result.movePos.set_x(tempDiff.y() * (wallRight ? +1 : -1));
            else if (!!(tempDirs & Dirs::DOWN) && wallDown)
                result.movePos.set_x(tempDiff.y() * (wallLeft ? +1 : -1));

            if (!!(tempDirs & Dirs::LEFT) && wallLeft)
                result.movePos.set_y(tempDiff.x() * (wallDown ? +1 : -1));
            else if (!!(tempDirs & Dirs::RIGHT) && wallRight)
                result.movePos.set_y(tempDiff.x() * (wallUp ? +1 : -1));
        }
    }

    _entity.setPosition(_entity.getPosition() - tempDiff);

    return result;
}

} // namespace ut::game::cpnt
