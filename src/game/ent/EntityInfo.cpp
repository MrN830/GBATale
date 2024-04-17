#include "game/ent/EntityInfo.hpp"

#include <bn_unordered_map.h>

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
#include "game/cpnt/inter/TalkFroggit.hpp"
#include "game/cpnt/inter/TalkNpcArea1.hpp"
#include "game/cpnt/inter/TalkTorielRuins3.hpp"
#include "game/cpnt/inter/TalkTorielRuins6.hpp"
#include "game/cpnt/inter/TorielGoOutRuins2.hpp"
#include "game/cpnt/inter/TorielGoOutRuins5.hpp"
#include "game/cpnt/inter/TorielGoOutRuins6.hpp"
#include "game/sys/CameraManager.hpp"
#include "game/sys/EntityManager.hpp"

namespace ut::game::ent
{

namespace
{

using InteractionFactoryHashMap = bn::unordered_map<
    bn::type_id_t, cpnt::inter::Interaction& (*)(ent::Entity&, const EntityInfo::Interaction&, bn::best_fit_allocator&),
    512>;

using EventComponentFactoryHashMap = bn::unordered_map<
    bn::type_id_t,
    cpnt::ev::EventComponent& (*)(ent::Entity&, const EntityInfo::EventComponent&, bn::best_fit_allocator&), 512>;

template <typename TInter>
    requires std::is_base_of_v<cpnt::inter::Interaction, TInter>
auto createChildInteraction(ent::Entity& entity, const EntityInfo::Interaction& interInfo,
                            bn::best_fit_allocator& cpntHeap) -> cpnt::inter::Interaction&
{
    return cpntHeap.create<TInter>(entity, interInfo.isEnabled, interInfo.triggers);
}

template <typename... TEmpty>
    requires(sizeof...(TEmpty) == 0)
void impl_createInteractionFactoryHashMap(InteractionFactoryHashMap&)
{
}

template <typename TInter, typename... TParams>
    requires std::is_base_of_v<cpnt::inter::Interaction, TInter>
void impl_createInteractionFactoryHashMap(InteractionFactoryHashMap& result)
{
    result[bn::type_id<TInter>()] = createChildInteraction<TInter>;

    impl_createInteractionFactoryHashMap<TParams...>(result);
}

template <typename... TParams>
auto createInteractionFactoryHashMap() -> InteractionFactoryHashMap
{
    InteractionFactoryHashMap result;
    impl_createInteractionFactoryHashMap<TParams...>(result);

    return result;
}

template <typename TEventCpnt>
    requires std::is_base_of_v<cpnt::ev::EventComponent, TEventCpnt>
auto createChildEventComponent(ent::Entity& entity, const EntityInfo::EventComponent& evInfo,
                               bn::best_fit_allocator& cpntHeap) -> cpnt::ev::EventComponent&
{
    return cpntHeap.create<TEventCpnt>(entity, evInfo.isEnabled, evInfo.isAutoFire);
}

template <typename... TEmpty>
    requires(sizeof...(TEmpty) == 0)
void impl_createEventComponentFactoryHashMap(EventComponentFactoryHashMap&)
{
}

template <typename TEventCpnt, typename... TParams>
    requires std::is_base_of_v<cpnt::ev::EventComponent, TEventCpnt>
void impl_createEventComponentFactoryHashMap(EventComponentFactoryHashMap& result)
{
    result[bn::type_id<TEventCpnt>()] = createChildEventComponent<TEventCpnt>;

    impl_createEventComponentFactoryHashMap<TParams...>(result);
}

template <typename... TParams>
auto createEventComponentFactoryHashMap() -> EventComponentFactoryHashMap
{
    EventComponentFactoryHashMap result;
    impl_createEventComponentFactoryHashMap<TParams...>(result);

    return result;
}

} // namespace

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

        BN_DATA_EWRAM static const InteractionFactoryHashMap FACTORIES = createInteractionFactoryHashMap<
            SavePoint, AutoHideSpike, RuinsFloorSwitch, RuinsWallSwitch, RuinsColorSwitch, RuinsColorSwitchHelp,
            Readable, TalkFroggit, ItemPickup, HoleFall, HoleUp, MouseSqueak, CutsceneRuins2, TorielGoOutRuins2,
            TalkTorielRuins3, TalkTorielRuins6, TorielGoOutRuins5, TorielGoOutRuins6, RuinsTorielCall, CutsceneRuins19,
            Chairiel, AsrielLamp, CutsceneBasement1Block, CutsceneBasement1Proceed, CutsceneBasement2,
            CutsceneBasement3, TalkNpcArea1, FloweyTrigger2>();

        auto it = FACTORIES.find(interaction->type);
        BN_ASSERT(it != FACTORIES.cend() && it->second != nullptr,
                  "Invalid interaction->type = ", (void*)interaction->type.internal_id());

        auto& inter = it->second(entity, *interaction, entMngr._cpntHeap);
        entity.addComponent(inter);
    }

    // child of `cpnt::ev::EventComponent`
    if (this->eventCpnt.has_value())
    {
        using namespace cpnt::ev;

        BN_DATA_EWRAM static const EventComponentFactoryHashMap FACTORIES =
            createEventComponentFactoryHashMap<StartBgm, TimedDestroy, PlotSpike, SetPieImage, StalkerFlowey,
                                               TorielGoOutRuins1, CutsceneRuins3, CutsceneRuins5, CutsceneRuins6,
                                               CutsceneTorhouse1, CutsceneTorhouse3>();

        auto it = FACTORIES.find(eventCpnt->type);
        BN_ASSERT(it != FACTORIES.cend() && it->second != nullptr,
                  "Invalid eventCpnt->type = ", (void*)eventCpnt->type.internal_id());

        auto& evCpnt = it->second(entity, *eventCpnt, entMngr._cpntHeap);
        entity.addComponent(evCpnt);
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
