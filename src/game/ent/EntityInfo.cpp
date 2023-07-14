#include "game/ent/EntityInfo.hpp"

#include "game/GameContext.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/sys/CameraManager.hpp"
#include "game/sys/EntityManager.hpp"

namespace ut::game::ent
{

void EntityInfo::create(GameContext& ctx) const
{
    auto& entMngr = ctx.entMngr;

    ent::Entity& entity = entMngr._entPool.create(this->id);
    entMngr._entities.push_front(entity);

    entity.setPosition(this->position);

    if (this->sprite.has_value())
    {
        cpnt::Sprite& spr = entMngr._cpntHeap.create<cpnt::Sprite>(
            entity, sprite->sprItem, sprite->gfxIdx, &ctx.camMngr.getCamera(), sprite->isMoving, sprite->zOrder);
        entity.addComponent(spr);

        spr.setEnabled(sprite->isEnabled);
    }
}

} // namespace ut::game::ent
