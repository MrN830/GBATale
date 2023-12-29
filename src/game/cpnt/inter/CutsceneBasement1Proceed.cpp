#include "game/cpnt/inter/CutsceneBasement1Proceed.hpp"

#include "core/MsgViewHolder.hpp"
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

CutsceneBasement1Proceed::CutsceneBasement1Proceed(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<CutsceneBasement1Proceed>(), isEnabled, triggers)
{
}

void CutsceneBasement1Proceed::awake(GameContext& ctx)
{
    const auto plot = ctx.state.getPlot();

    if (plot <= GamePlot::I_HAVE_TO_DO_SOMETHING)
        _entity.setDestroyed(true);
    else
    {
        _toriel = ctx.entMngr.findById(ent::gen::EntityId::toriel);
        BN_ASSERT(_toriel);
        _torielInput = _toriel->getComponent<NpcInput>();
        BN_ASSERT(_torielInput);
        _torielWalk = _toriel->getComponent<WalkAnimCtrl>();
        BN_ASSERT(_torielWalk);

        if (plot > GamePlot::TORIEL_IN_BASEMENT)
        {
            _entity.setDestroyed(true);
            _toriel->setDestroyed(true);
        }
        else
            _torielWalk->setStandStillDir(core::Directions::RIGHT);
    }
}

auto CutsceneBasement1Proceed::onInteract(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    Interaction::onInteract(ctx);

    // Do not interact twice
    setEnabled(false);

    // Toriel talks
    ctx.msg.getSettings() = core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
    ctx.msg.getSettings().emotion = 2;
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::obj_torieltrigger9_77);
    ctx.msg.add(gen::TextData::obj_torieltrigger9_78);
    ctx.msg.add(gen::TextData::obj_torieltrigger9_79);
    ctx.msg.add(gen::TextData::obj_torieltrigger9_80);
    ctx.msg.add(gen::TextData::obj_torieltrigger9_81);
    ctx.msg.add(gen::TextData::obj_torieltrigger9_82);
    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    ctx.msg.getSettings().reset();
    ctx.state.setPlot(GamePlot::TORIEL_IN_BASEMENT_2);

    // Toriel walks out of the room
    cmd::MoveCmd moveCmd{
        .directions = core::Directions::RIGHT,
        .movePos = {3.2, 0},
        .checkCollision = false,
    };
    _torielInput->startOneWay(moveCmd, 2 * 30);
    co_await task::SignalAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

    _toriel->setDestroyed(true);
    _entity.setDestroyed(true);
    co_return;
}

} // namespace ut::game::cpnt::inter
