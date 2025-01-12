#include "game/ent/EntityInfo.hpp"

#include "game/GameContext.hpp"
#include "game/sys/CameraManager.hpp"
#include "game/sys/EntityManager.hpp"

#include "game/cpnt/ev/EventComponentType.hpp"
#include "game/cpnt/inter/InteractionType.hpp"

#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/cpnt/ev/CutsceneRuins3.hpp"
#include "game/cpnt/ev/CutsceneRuins4.hpp"
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
#include "game/cpnt/inter/FloweyTrigger1.hpp"
#include "game/cpnt/inter/FloweyTrigger2.hpp"
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
#include "game/cpnt/inter/TalkDummy.hpp"
#include "game/cpnt/inter/TalkFroggit.hpp"
#include "game/cpnt/inter/TalkNpcArea1.hpp"
#include "game/cpnt/inter/TalkTorielRuins3.hpp"
#include "game/cpnt/inter/TalkTorielRuins4.hpp"
#include "game/cpnt/inter/TalkTorielRuins6.hpp"
#include "game/cpnt/inter/TorielGoOutRuins2.hpp"
#include "game/cpnt/inter/TorielGoOutRuins5.hpp"
#include "game/cpnt/inter/TorielGoOutRuins6.hpp"

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
        using namespace cpnt::inter;

        cpnt::inter::Interaction* inter = nullptr;

        switch (interaction->type)
        {
        case InteractionType::SavePoint:
            inter = &entMngr._cpntHeap.create<SavePoint>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::AutoHideSpike:
            inter = &entMngr._cpntHeap.create<AutoHideSpike>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::RuinsFloorSwitch:
            inter = &entMngr._cpntHeap.create<RuinsFloorSwitch>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::RuinsWallSwitch:
            inter = &entMngr._cpntHeap.create<RuinsWallSwitch>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::RuinsColorSwitch:
            inter = &entMngr._cpntHeap.create<RuinsColorSwitch>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::RuinsColorSwitchHelp:
            inter =
                &entMngr._cpntHeap.create<RuinsColorSwitchHelp>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::Readable:
            inter = &entMngr._cpntHeap.create<Readable>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::TalkFroggit:
            inter = &entMngr._cpntHeap.create<TalkFroggit>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::ItemPickup:
            inter = &entMngr._cpntHeap.create<ItemPickup>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::HoleFall:
            inter = &entMngr._cpntHeap.create<HoleFall>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::HoleUp:
            inter = &entMngr._cpntHeap.create<HoleUp>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::MouseSqueak:
            inter = &entMngr._cpntHeap.create<MouseSqueak>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::CutsceneRuins2:
            inter = &entMngr._cpntHeap.create<CutsceneRuins2>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::TorielGoOutRuins2:
            inter = &entMngr._cpntHeap.create<TorielGoOutRuins2>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::TalkTorielRuins3:
            inter = &entMngr._cpntHeap.create<TalkTorielRuins3>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::TalkTorielRuins4:
            inter = &entMngr._cpntHeap.create<TalkTorielRuins4>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::TalkDummy:
            inter = &entMngr._cpntHeap.create<TalkDummy>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::TalkTorielRuins6:
            inter = &entMngr._cpntHeap.create<TalkTorielRuins6>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::TorielGoOutRuins5:
            inter = &entMngr._cpntHeap.create<TorielGoOutRuins5>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::TorielGoOutRuins6:
            inter = &entMngr._cpntHeap.create<TorielGoOutRuins6>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::RuinsTorielCall:
            inter = &entMngr._cpntHeap.create<RuinsTorielCall>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::CutsceneRuins19:
            inter = &entMngr._cpntHeap.create<CutsceneRuins19>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::Chairiel:
            inter = &entMngr._cpntHeap.create<Chairiel>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::AsrielLamp:
            inter = &entMngr._cpntHeap.create<AsrielLamp>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::CutsceneBasement1Block:
            inter = &entMngr._cpntHeap.create<CutsceneBasement1Block>(entity, interaction->isEnabled,
                                                                      interaction->triggers);
            break;
        case InteractionType::CutsceneBasement1Proceed:
            inter = &entMngr._cpntHeap.create<CutsceneBasement1Proceed>(entity, interaction->isEnabled,
                                                                        interaction->triggers);
            break;
        case InteractionType::CutsceneBasement2:
            inter = &entMngr._cpntHeap.create<CutsceneBasement2>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::CutsceneBasement3:
            inter = &entMngr._cpntHeap.create<CutsceneBasement3>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::TalkNpcArea1:
            inter = &entMngr._cpntHeap.create<TalkNpcArea1>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::FloweyTrigger1:
            inter = &entMngr._cpntHeap.create<FloweyTrigger1>(entity, interaction->isEnabled, interaction->triggers);
            break;
        case InteractionType::FloweyTrigger2:
            inter = &entMngr._cpntHeap.create<FloweyTrigger2>(entity, interaction->isEnabled, interaction->triggers);
            break;

        default:
            BN_ERROR(inter != nullptr, "Invalid interaction->type = ", (int)interaction->type);
        }

        BN_ASSERT(inter != nullptr, "Invalid interaction->type = ", (int)interaction->type);

        entity.addComponent(*inter);
    }

    // child of `cpnt::ev::EventComponent`
    if (this->eventCpnt.has_value())
    {
        using namespace cpnt::ev;

        cpnt::ev::EventComponent* evCpnt = nullptr;

        switch (eventCpnt->type)
        {
        case EventComponentType::StartBgm:
            evCpnt = &entMngr._cpntHeap.create<StartBgm>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
            break;
        case EventComponentType::TimedDestroy:
            evCpnt = &entMngr._cpntHeap.create<TimedDestroy>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
            break;
        case EventComponentType::PlotSpike:
            evCpnt = &entMngr._cpntHeap.create<PlotSpike>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
            break;
        case EventComponentType::SetPieImage:
            evCpnt = &entMngr._cpntHeap.create<SetPieImage>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
            break;
        case EventComponentType::StalkerFlowey:
            evCpnt = &entMngr._cpntHeap.create<StalkerFlowey>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
            break;
        case EventComponentType::TorielGoOutRuins1:
            evCpnt = &entMngr._cpntHeap.create<TorielGoOutRuins1>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
            break;
        case EventComponentType::CutsceneRuins3:
            evCpnt = &entMngr._cpntHeap.create<CutsceneRuins3>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
            break;
        case EventComponentType::CutsceneRuins4:
            evCpnt = &entMngr._cpntHeap.create<CutsceneRuins4>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
            break;
        case EventComponentType::CutsceneRuins5:
            evCpnt = &entMngr._cpntHeap.create<CutsceneRuins5>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
            break;
        case EventComponentType::CutsceneRuins6:
            evCpnt = &entMngr._cpntHeap.create<CutsceneRuins6>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
            break;
        case EventComponentType::CutsceneTorhouse1:
            evCpnt = &entMngr._cpntHeap.create<CutsceneTorhouse1>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
            break;
        case EventComponentType::CutsceneTorhouse3:
            evCpnt = &entMngr._cpntHeap.create<CutsceneTorhouse3>(entity, eventCpnt->isEnabled, eventCpnt->isAutoFire);
            break;

        default:
            BN_ERROR("Invalid eventCpnt->type = ", (int)eventCpnt->type);
        }

        BN_ASSERT(evCpnt != nullptr, "Invalid eventCpnt->type = ", (int)eventCpnt->type);

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
