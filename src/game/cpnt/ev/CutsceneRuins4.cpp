#include "game/cpnt/ev/CutsceneRuins4.hpp"

#include "asset/Path.hpp"
#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/cpnt/ev/EventComponentType.hpp"
#include "game/cpnt/inter/Interaction.hpp"
#include "game/ent/Entity.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::ev
{

CutsceneRuins4::CutsceneRuins4(ent::Entity& entity, bool isEnabled, bool isAutoFire)
    : EventComponent(entity, EventComponentType::CutsceneRuins4, isEnabled, isAutoFire)
{
}

void CutsceneRuins4::awake(GameContext& ctx)
{
    if (ctx.state.getPlot() >= GamePlot::TORIEL_AT_SPIKE_PUZZLE)
        _entity.setDestroyed(true);
}

auto CutsceneRuins4::onEvent(GameContext& ctx) -> task::Task
{
    auto* torielInteract = _entity.getComponent<inter::Interaction>();
    BN_ASSERT(torielInteract);

    const auto plot = ctx.state.getPlot();

    if (plot >= GamePlot::TORIEL_WAITING_AT_DUMMY)
    {
        // warp Toriel to final pos
        const auto pos = asset::IPath::get(asset::gen::PathId::path_torielwalk4).getPoints().back();
        _entity.setPosition(pos);

        torielInteract->setEnabled(true);
    }
    else
    {
        using namespace ut::asset;

        auto* torielWalk = _entity.getComponent<WalkAnimCtrl>();
        BN_ASSERT(torielWalk);
        auto* torielInput = _entity.getComponent<NpcInput>();
        BN_ASSERT(torielInput);

        ctx.msg.getSettings() =
            core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
        // TODO: Fix this, it doesn't work because `CameraManager::update()` sets it
        ctx.isDialogUpper = false; // Force lower dialog box, otherwise Toriel is hidden

        // Toriel talks
        ctx.msg.clearMsg();
        ctx.msg.add(gen::TextData::SCR_TEXT_367);
        ctx.msg.add(gen::TextData::SCR_TEXT_368);
        ctx.msg.add(gen::TextData::SCR_TEXT_369);
        ctx.msg.add(gen::TextData::SCR_TEXT_370);
        ctx.msg.add(gen::TextData::SCR_TEXT_371);
        ctx.msg.add(gen::TextData::SCR_TEXT_372);
        ctx.msg.add(gen::TextData::SCR_TEXT_373);
        ctx.game.startDialog();
        co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

        ctx.msg.getSettings().reset();

        // Toriel walks
        torielInput->startPath(IPath::get(gen::PathId::path_torielwalk4), 3.2);
        co_await task::SignalAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

        torielWalk->setStandStillDir(core::Directions::DOWN);
        torielInteract->setEnabled(true);
        ctx.state.setPlot(GamePlot::TORIEL_WAITING_AT_DUMMY);
    }

    co_return;
}

} // namespace ut::game::cpnt::ev
