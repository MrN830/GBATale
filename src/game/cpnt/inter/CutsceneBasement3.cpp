#include "game/cpnt/inter/CutsceneBasement3.hpp"

#include "core/DialogSettings.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/EntityManager.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

CutsceneBasement3::CutsceneBasement3(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<CutsceneBasement3>(), isEnabled, triggers)
{
}

void CutsceneBasement3::awake(GameContext& ctx)
{
    _toriel = ctx.entMngr.findById(ent::gen::EntityId::toriel);
    BN_ASSERT(_toriel);

    if (ctx.state.getPlot() > GamePlot::TORIEL_IN_BASEMENT_3)
    {
        _toriel->setDestroyed(true);
        _entity.setDestroyed(true);
    }
    else
    {
        _torielInput = _toriel->getComponent<NpcInput>();
        BN_ASSERT(_torielInput);
        _torielWalk = _toriel->getComponent<WalkAnimCtrl>();
        BN_ASSERT(_torielWalk);

        _torielWalk->setStandStillDir(core::Directions::UP);
    }
}

auto CutsceneBasement3::onInteract(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    Interaction::onInteract(ctx);

    // Don't interact twice
    setEnabled(false);

    // Toriel talks
    ctx.msgSettings = core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
    ctx.msgSettings.emotion = 6;
    ctx.msg.clear();
    ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger11_79));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger11_80));
    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    ctx.msgSettings.reset();
    ctx.state.setPlot(GamePlot::TORIEL_AT_RUINS_EXIT);

    // Toriel walks out of the room
    cmd::MoveCmd moveCmd{
        .directions = core::Directions::UP,
        .movePos = {0, -3.2},
        .checkCollision = false,
    };
    _torielInput->startOneWay(moveCmd, 3 * 30);
    co_await task::SignalAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

    _toriel->setDestroyed(true);
    _entity.setDestroyed(true);
    co_return;
}

} // namespace ut::game::cpnt::inter
