#include "game/ent/EntityInfo.hpp"

#include "game/GameContext.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/cpnt/ev/StartBgm.hpp"
#include "game/cpnt/inter/AutoHideSpike.hpp"
#include "game/cpnt/inter/HoleFall.hpp"
#include "game/cpnt/inter/HoleUp.hpp"
#include "game/cpnt/inter/ItemPickup.hpp"
#include "game/cpnt/inter/PlotSpike.hpp"
#include "game/cpnt/inter/Readable.hpp"
#include "game/cpnt/inter/RuinsColorSwitch.hpp"
#include "game/cpnt/inter/RuinsColorSwitchHelp.hpp"
#include "game/cpnt/inter/RuinsFloorSwitch.hpp"
#include "game/cpnt/inter/SavePoint.hpp"
#include "game/cpnt/inter/TalkFroggit.hpp"
#include "game/sys/CameraManager.hpp"
#include "game/sys/EntityManager.hpp"

namespace ut::game::ent
{

void EntityInfo::create(GameContext& ctx) const
{
    auto& entMngr = ctx.entMngr;

    ent::Entity& entity = entMngr._entPool.create(this->id, this->position);
    entMngr._entities.push_front(entity);

    // cpnt::ColliderPack
    if (this->collPack.has_value())
    {
        cpnt::ColliderPack& collPackCpnt =
            entMngr._cpntHeap.create<cpnt::ColliderPack>(entity, collPack->isEnabled, collPack->isTrigger);
        entity.addComponent(collPackCpnt);

        collPackCpnt.setStaticCollInfos(collPack->staticCollInfos);
    }

    // child of `cpnt::inter::Interaction`
    if (this->interaction.has_value())
    {
        cpnt::inter::Interaction* inter = nullptr;

        if (interaction->type == bn::type_id<cpnt::inter::SavePoint>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::SavePoint>(entity, interaction->isEnabled,
                                                                      interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::AutoHideSpike>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::AutoHideSpike>(entity, interaction->isEnabled,
                                                                          interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::PlotSpike>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::PlotSpike>(entity, interaction->isEnabled,
                                                                      interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::RuinsFloorSwitch>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::RuinsFloorSwitch>(entity, interaction->isEnabled,
                                                                             interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::RuinsColorSwitch>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::RuinsColorSwitch>(entity, interaction->isEnabled,
                                                                             interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::RuinsColorSwitchHelp>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::RuinsColorSwitchHelp>(entity, interaction->isEnabled,
                                                                                 interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::Readable>())
            inter =
                &entMngr._cpntHeap.create<cpnt::inter::Readable>(entity, interaction->isEnabled, interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::TalkFroggit>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::TalkFroggit>(entity, interaction->isEnabled,
                                                                        interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::ItemPickup>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::ItemPickup>(entity, interaction->isEnabled,
                                                                       interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::HoleFall>())
            inter =
                &entMngr._cpntHeap.create<cpnt::inter::HoleFall>(entity, interaction->isEnabled, interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::HoleUp>())
            inter =
                &entMngr._cpntHeap.create<cpnt::inter::HoleUp>(entity, interaction->isEnabled, interaction->triggers);
        else
            BN_ERROR("Invalid interaction->type = ", (void*)interaction->type.internal_id());

        entity.addComponent(*inter);
    }

    // child of `cpnt::ev::EventComponent`
    if (this->eventCpnt.has_value())
    {
        cpnt::ev::EventComponent* evCpnt = nullptr;

        if (eventCpnt->type == bn::type_id<cpnt::ev::StartBgm>())
            evCpnt = &entMngr._cpntHeap.create<cpnt::ev::StartBgm>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
        else
            BN_ERROR("Invalid eventCpnt->type = ", (void*)eventCpnt->type.internal_id());

        entity.addComponent(*evCpnt);
    }

    // cpnt::Sprite
    if (this->sprite.has_value())
    {
        cpnt::Sprite& spr = entMngr._cpntHeap.create<cpnt::Sprite>(
            entity, sprite->isEnabled, sprite->sprItem, sprite->gfxIdx, sprite->isBlendingEnabled,
            &ctx.camMngr.getCamera(), sprite->isMoving, sprite->bgPriority, sprite->zOrder);
        entity.addComponent(spr);

        // cpnt::SpriteAnim
        if (this->sprAnim.has_value())
        {
            cpnt::SpriteAnim& sprAnimCpnt = entMngr._cpntHeap.create<cpnt::SpriteAnim>(entity, sprAnim->isEnabled, spr);
            entity.addComponent(sprAnimCpnt);

            sprAnimCpnt.setCurAnimKind(sprAnim->kind);

            // cpnt::WalkAnimCtrl
            if (this->walkAnimCtrl.has_value())
            {
                cpnt::WalkAnimCtrl& walk =
                    entMngr._cpntHeap.create<cpnt::WalkAnimCtrl>(entity, walkAnimCtrl->isEnabled, sprAnimCpnt);
                entity.addComponent(walk);

                walk.registerWalkAnimKind(walkAnimCtrl->kind);
            }
        }
    }
}

} // namespace ut::game::ent
