#include "game/ent/EntityInfo.hpp"

#include "game/GameContext.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/cpnt/ev/CutsceneRuins3.hpp"
#include "game/cpnt/ev/CutsceneRuins5.hpp"
#include "game/cpnt/ev/CutsceneRuins6.hpp"
#include "game/cpnt/ev/CutsceneTorhouse1.hpp"
#include "game/cpnt/ev/CutsceneTorhouse3.hpp"
#include "game/cpnt/ev/PlotSpike.hpp"
#include "game/cpnt/ev/SetPieImage.hpp"
#include "game/cpnt/ev/StalkerFlowey.hpp"
#include "game/cpnt/ev/StartBgm.hpp"
#include "game/cpnt/ev/TimedDestroy.hpp"
#include "game/cpnt/ev/TorielGoOutRuins1.hpp"
#include "game/cpnt/inter/AsrielLamp.hpp"
#include "game/cpnt/inter/AutoHideSpike.hpp"
#include "game/cpnt/inter/Chairiel.hpp"
#include "game/cpnt/inter/CutsceneBasement1Block.hpp"
#include "game/cpnt/inter/CutsceneBasement1Proceed.hpp"
#include "game/cpnt/inter/CutsceneBasement2.hpp"
#include "game/cpnt/inter/CutsceneBasement3.hpp"
#include "game/cpnt/inter/CutsceneRuins19.hpp"
#include "game/cpnt/inter/CutsceneRuins2.hpp"
#include "game/cpnt/inter/HoleFall.hpp"
#include "game/cpnt/inter/HoleUp.hpp"
#include "game/cpnt/inter/ItemPickup.hpp"
#include "game/cpnt/inter/MouseSqueak.hpp"
#include "game/cpnt/inter/Readable.hpp"
#include "game/cpnt/inter/RuinsColorSwitch.hpp"
#include "game/cpnt/inter/RuinsColorSwitchHelp.hpp"
#include "game/cpnt/inter/RuinsFloorSwitch.hpp"
#include "game/cpnt/inter/RuinsTorielCall.hpp"
#include "game/cpnt/inter/RuinsWallSwitch.hpp"
#include "game/cpnt/inter/SavePoint.hpp"
#include "game/cpnt/inter/TalkFroggit.hpp"
#include "game/cpnt/inter/TalkTorielRuins3.hpp"
#include "game/cpnt/inter/TalkTorielRuins6.hpp"
#include "game/cpnt/inter/TorielGoOutRuins2.hpp"
#include "game/cpnt/inter/TorielGoOutRuins5.hpp"
#include "game/cpnt/inter/TorielGoOutRuins6.hpp"
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
        else if (interaction->type == bn::type_id<cpnt::inter::RuinsFloorSwitch>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::RuinsFloorSwitch>(entity, interaction->isEnabled,
                                                                             interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::RuinsWallSwitch>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::RuinsWallSwitch>(entity, interaction->isEnabled,
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
        else if (interaction->type == bn::type_id<cpnt::inter::MouseSqueak>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::MouseSqueak>(entity, interaction->isEnabled,
                                                                        interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::CutsceneRuins2>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::CutsceneRuins2>(entity, interaction->isEnabled,
                                                                           interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::TorielGoOutRuins2>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::TorielGoOutRuins2>(entity, interaction->isEnabled,
                                                                              interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::TalkTorielRuins3>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::TalkTorielRuins3>(entity, interaction->isEnabled,
                                                                             interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::TalkTorielRuins6>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::TalkTorielRuins6>(entity, interaction->isEnabled,
                                                                             interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::TorielGoOutRuins5>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::TorielGoOutRuins5>(entity, interaction->isEnabled,
                                                                              interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::TorielGoOutRuins6>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::TorielGoOutRuins6>(entity, interaction->isEnabled,
                                                                              interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::RuinsTorielCall>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::RuinsTorielCall>(entity, interaction->isEnabled,
                                                                            interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::CutsceneRuins19>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::CutsceneRuins19>(entity, interaction->isEnabled,
                                                                            interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::Chairiel>())
            inter =
                &entMngr._cpntHeap.create<cpnt::inter::Chairiel>(entity, interaction->isEnabled, interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::AsrielLamp>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::AsrielLamp>(entity, interaction->isEnabled,
                                                                       interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::CutsceneBasement1Block>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::CutsceneBasement1Block>(entity, interaction->isEnabled,
                                                                                   interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::CutsceneBasement1Proceed>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::CutsceneBasement1Proceed>(entity, interaction->isEnabled,
                                                                                     interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::CutsceneBasement2>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::CutsceneBasement2>(entity, interaction->isEnabled,
                                                                              interaction->triggers);
        else if (interaction->type == bn::type_id<cpnt::inter::CutsceneBasement3>())
            inter = &entMngr._cpntHeap.create<cpnt::inter::CutsceneBasement3>(entity, interaction->isEnabled,
                                                                              interaction->triggers);
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
        else if (eventCpnt->type == bn::type_id<cpnt::ev::TimedDestroy>())
            evCpnt =
                &entMngr._cpntHeap.create<cpnt::ev::TimedDestroy>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
        else if (eventCpnt->type == bn::type_id<cpnt::ev::PlotSpike>())
            evCpnt =
                &entMngr._cpntHeap.create<cpnt::ev::PlotSpike>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
        else if (eventCpnt->type == bn::type_id<cpnt::ev::SetPieImage>())
            evCpnt =
                &entMngr._cpntHeap.create<cpnt::ev::SetPieImage>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
        else if (eventCpnt->type == bn::type_id<cpnt::ev::StalkerFlowey>())
            evCpnt =
                &entMngr._cpntHeap.create<cpnt::ev::StalkerFlowey>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
        else if (eventCpnt->type == bn::type_id<cpnt::ev::TorielGoOutRuins1>())
            evCpnt = &entMngr._cpntHeap.create<cpnt::ev::TorielGoOutRuins1>(entity, eventCpnt->isEnabled,
                                                                            eventCpnt->isAutoFire);
        else if (eventCpnt->type == bn::type_id<cpnt::ev::CutsceneRuins3>())
            evCpnt = &entMngr._cpntHeap.create<cpnt::ev::CutsceneRuins3>(entity, eventCpnt->isEnabled,
                                                                         eventCpnt->isAutoFire);
        else if (eventCpnt->type == bn::type_id<cpnt::ev::CutsceneRuins5>())
            evCpnt = &entMngr._cpntHeap.create<cpnt::ev::CutsceneRuins5>(entity, eventCpnt->isEnabled,
                                                                         eventCpnt->isAutoFire);
        else if (eventCpnt->type == bn::type_id<cpnt::ev::CutsceneRuins6>())
            evCpnt = &entMngr._cpntHeap.create<cpnt::ev::CutsceneRuins6>(entity, eventCpnt->isEnabled,
                                                                         eventCpnt->isAutoFire);
        else if (eventCpnt->type == bn::type_id<cpnt::ev::CutsceneTorhouse1>())
            evCpnt = &entMngr._cpntHeap.create<cpnt::ev::CutsceneTorhouse1>(entity, eventCpnt->isEnabled,
                                                                            eventCpnt->isAutoFire);
        else if (eventCpnt->type == bn::type_id<cpnt::ev::CutsceneTorhouse3>())
            evCpnt = &entMngr._cpntHeap.create<cpnt::ev::CutsceneTorhouse3>(entity, eventCpnt->isEnabled,
                                                                            eventCpnt->isAutoFire);
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

    // cpnt::NpcInput
    if (this->npcInput.has_value())
    {
        cpnt::NpcInput& npcMove = entMngr._cpntHeap.create<cpnt::NpcInput>(entity, npcInput->isEnabled);
        entity.addComponent(npcMove);
    }
}

} // namespace ut::game::ent
