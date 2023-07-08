#include "game/cpnt/Input.hpp"

#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/cmd/InputCmd.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/EntityManager.hpp"
#include "mtile/MTilemap.hpp"

namespace ut::game::cpnt
{

Input::Input(ent::Entity& entity) : Component(entity)
{
}

void Input::sendInput(const cmd::InputCmd& cmd, GameContext& ctx)
{
    const auto collFixedCmd = (cmd.checkCollision ? fixMoveCmdCollision(cmd, ctx) : cmd);

    // translate entity position
    _entity.setPosition(_entity.getPosition() + collFixedCmd.movePos);

    // send `cmd` to `cpnt::SpriteAnim`
    auto* sprAnim = _entity.getComponent<cpnt::SpriteAnim>();
    if (sprAnim != nullptr)
        sprAnim->receiveInputCmd(collFixedCmd);
}

auto Input::fixMoveCmdCollision(const cmd::InputCmd& cmd, GameContext& ctx) -> cmd::InputCmd
{
    const auto* collPack = _entity.getComponent<ColliderPack>();
    if (collPack == nullptr || collPack->isTrigger())
        return cmd;

    auto result = cmd;

    const auto* mTilemap = getRoomMTilemap(ctx.state.getRoom());
    BN_ASSERT(mTilemap != nullptr);
    const auto& entities = ctx.entMngr.getEntities();

    auto hasStopCollision = [this, collPack, mTilemap, &entities]() -> bool {
        // rect wall collision
        for (const auto& wall : mTilemap->getRectWalls())
            if (collPack->isCollideWith(wall))
                return true;

        // other entity collision
        for (auto eIt = entities.cbegin(); eIt != entities.cend(); ++eIt)
        {
            if (&*eIt == &_entity)
                continue;

            const auto* otherColl = eIt->getComponent<ColliderPack>();
            if (otherColl == nullptr)
                continue;

            if (!otherColl->isTrigger() && collPack->isCollideWith(*otherColl))
                return true;
        }

        return false;
    };

    // stop walls
    if (cmd.movePos.x() != 0)
    {
        const bn::fixed_point tempDiff{cmd.movePos.x(), 0};
        _entity.setPosition(_entity.getPosition() + tempDiff);
        if (hasStopCollision())
            result.movePos.set_x(0);
        _entity.setPosition(_entity.getPosition() - tempDiff);
    }
    if (cmd.movePos.y() != 0)
    {
        const bn::fixed_point tempDiff{0, cmd.movePos.y()};
        _entity.setPosition(_entity.getPosition() + tempDiff);
        if (hasStopCollision())
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
