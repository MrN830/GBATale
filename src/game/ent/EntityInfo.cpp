#include "game/ent/EntityInfo.hpp"

#include "game/GameContext.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/sys/CameraManager.hpp"
#include "game/sys/EntityManager.hpp"

namespace ut::game::ent
{

void EntityInfo::create(GameContext& ctx) const
{
    auto& entMngr = ctx.entMngr;

    ent::Entity& entity = entMngr._entPool.create(this->id, this->position);
    entMngr._entities.push_front(entity);

    // cpnt::Sprite
    if (this->sprite.has_value())
    {
        cpnt::Sprite& spr =
            entMngr._cpntHeap.create<cpnt::Sprite>(entity, sprite->sprItem, sprite->gfxIdx, &ctx.camMngr.getCamera(),
                                                   sprite->isMoving, sprite->bgPriority, sprite->zOrder);
        entity.addComponent(spr);

        spr.setEnabled(sprite->isEnabled);

        // cpnt::SpriteAnim
        if (this->sprAnim.has_value())
        {
            cpnt::SpriteAnim& sprAnimCpnt = entMngr._cpntHeap.create<cpnt::SpriteAnim>(entity, spr);
            entity.addComponent(sprAnimCpnt);

            sprAnimCpnt.setEnabled(sprAnim->isEnabled);
            sprAnimCpnt.setCurAnimKind(sprAnim->kind);

            // cpnt::WalkAnimCtrl
            if (this->walkAnimCtrl.has_value())
            {
                cpnt::WalkAnimCtrl& walk = entMngr._cpntHeap.create<cpnt::WalkAnimCtrl>(entity, sprAnimCpnt);
                entity.addComponent(walk);

                walk.setEnabled(walkAnimCtrl->isEnabled);
                walk.registerWalkAnimKind(walkAnimCtrl->kind);
            }
        }
    }
}

} // namespace ut::game::ent
