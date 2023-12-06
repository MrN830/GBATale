#include "game/cpnt/ev/CutsceneTorhouse1.hpp"

#include "asset/Path.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/ent/Entity.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::ev
{

CutsceneTorhouse1::CutsceneTorhouse1(ent::Entity& entity, bool isEnabled, bool isAutoFire)
    : EventComponent(entity, bn::type_id<CutsceneTorhouse1>(), isEnabled, isAutoFire)
{
    BN_ASSERT(_entity.getId() == ent::gen::EntityId::toriel);
}

void CutsceneTorhouse1::awake(GameContext& ctx)
{
    const auto plot = ctx.state.getPlot();
    auto& flags = ctx.state.getFlags();

    if (flags.status_toriel != GameFlags::StatusToriel::LEAVE_RUINS && plot >= GamePlot::TORIEL_WAITING_AT_BEDROOMS)
        _entity.setDestroyed(true);
}

auto CutsceneTorhouse1::onEvent(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    const auto plot = ctx.state.getPlot();
    auto& flags = ctx.state.getFlags();

    auto* torielInput = _entity.getComponent<NpcInput>();
    BN_ASSERT(torielInput);

    task::SignalAwaiter torielWalkAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

    if (flags.status_toriel == GameFlags::StatusToriel::LEAVE_RUINS)
    {
        BN_ASSERT(ctx.interactStack.top() == InteractState::CUTSCENE);
        ctx.interactStack.pop();

        flags.status_toriel = GameFlags::StatusToriel::INIT;

        if (plot <= GamePlot::TORIEL_WAITING_AT_BEDROOMS)
            torielInput->startPath(IPath::get(gen::PathId::path_torielwalkhouse2), 3.2);
        else
            torielInput->startPath(IPath::get(gen::PathId::path_torielwalkhouse3), 3.2);

        co_await torielWalkAwaiter;
    }
    else if (plot < GamePlot::TORIEL_WAITING_AT_BEDROOMS)
    {
        // Toriel talks
        ctx.msgSettings =
            core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
        ctx.msg.clear();
        if (flags.hardmode)
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_49));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_50));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_51));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_52));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_53));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_54));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_55));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_56));
        }
        else
        {
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_39));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_40));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_41));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_42));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_43));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_44));
            ctx.msg.push_back(gen::getTextEn(gen::TextData::obj_torieltrigger6_45));
        }
        ctx.game.startDialog();
        co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

        ctx.msgSettings.reset();
        ctx.state.setPlot(GamePlot::TORIEL_WAITING_AT_BEDROOMS);

        // Toriel walks out of the room
        cmd::MoveCmd moveCmd{
            .directions = core::Directions::RIGHT,
            .movePos = {3.2, 0},
            .checkCollision = false,
        };
        torielInput->startOneWay(moveCmd, 35);
        co_await torielWalkAwaiter;
    }
    else
        BN_ERROR("Should not reach here, this entity should be destroyed before");

    _entity.setDestroyed(true);
    co_return;
}

} // namespace ut::game::cpnt::ev
