#include "game/cpnt/ev/CutsceneRuins3.hpp"

#include "asset/Path.hpp"
#include "core/DialogSettings.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/ColliderPack.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/cpnt/inter/Interaction.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/EntityManager.hpp"
#include "game/sys/TaskManager.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::ev
{

CutsceneRuins3::CutsceneRuins3(ent::Entity& entity, bool isEnabled, bool isAutoFire)
    : EventComponent(entity, bn::type_id<CutsceneRuins3>(), isEnabled, isAutoFire)
{
    BN_ASSERT(_entity.getId() == ent::gen::EntityId::toriel);
}

void CutsceneRuins3::awake(GameContext& ctx)
{
    auto* torielWalk = _entity.getComponent<WalkAnimCtrl>();
    BN_ASSERT(torielWalk);
    auto* torielInteract = _entity.getComponent<inter::Interaction>();
    BN_ASSERT(torielInteract);

    const auto plot = ctx.state.getPlot();

    if (plot >= GamePlot::SWITCH_PUZZLE_COMPLETE)
        _entity.setDestroyed(true);
    else if (plot >= GamePlot::FIRST_SWITCH_FLIPPED)
    {
        // warp Toriel to final pos
        const auto pos = asset::IPath::get(asset::gen::PathId::path_torielwalk3_2).getPoints().back();
        _entity.setPosition(pos);
        torielWalk->setStandStillDir(core::Directions::LEFT);

        torielInteract->setEnabled(true);
        setEnabled(false);
    }
    else
    {
        auto* plotWall = ctx.entMngr.findById(ent::gen::EntityId::plot_wall);
        BN_ASSERT(plotWall);
        auto* plotWallColl = plotWall->getComponent<ColliderPack>();
        BN_ASSERT(plotWallColl);

        plotWallColl->setEnabled(true);

        if (plot >= GamePlot::TORIEL_WAITING_AT_SWITCH_PUZZLE)
        {
            // warp Toriel to 2nd bridge
            const auto pos = asset::IPath::get(asset::gen::PathId::path_torielwalk3).getPoints().back();
            _entity.setPosition(pos);
            torielWalk->setStandStillDir(core::Directions::LEFT);

            torielInteract->setEnabled(true);
            setEnabled(false);
        }
    }

    _frisk = ctx.entMngr.findById(ent::gen::EntityId::frisk);
    BN_ASSERT(_frisk);
}

void CutsceneRuins3::update(GameContext& ctx)
{
    EventComponent::update(ctx);

    if (_hasFired)
        return;

    const auto& friskPos = _frisk->getPosition();

    if (friskPos.x() > 122 || friskPos.y() < 136 + 23)
    {
        _hasFired = true;

        auto task = onEvent(ctx);
        if (!task.done())
            ctx.taskMngr.addTask(std::move(task));
    }
}

auto CutsceneRuins3::onEvent(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    auto* torielWalk = _entity.getComponent<WalkAnimCtrl>();
    BN_ASSERT(torielWalk);
    auto* torielInput = _entity.getComponent<NpcInput>();
    BN_ASSERT(torielInput);
    auto* torielInteract = _entity.getComponent<inter::Interaction>();
    BN_ASSERT(torielInteract);

    ctx.msgSettings = core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);

    // Toriel talks
    ctx.msg.clear();
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_318));
    if (ctx.state.getFlags().hardmode)
    {
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_322));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_323));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_324));
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_325));
    }
    else
    {
        ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_319));
    }
    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    ctx.msgSettings.reset();

    // Toriel walks to 2nd bridge
    torielInput->startPath(IPath::get(gen::PathId::path_torielwalk3), 2.6);
    co_await task::SignalAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

    torielWalk->setStandStillDir(core::Directions::LEFT);

    // recheck, since the player could have flipped the switch already
    if (ctx.state.getPlot() < GamePlot::TORIEL_WAITING_AT_SWITCH_PUZZLE)
        ctx.state.setPlot(GamePlot::TORIEL_WAITING_AT_SWITCH_PUZZLE);
    torielInteract->setEnabled(true);
    setEnabled(false);
    co_return;
}

} // namespace ut::game::cpnt::ev
