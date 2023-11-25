#include "game/cpnt/inter/CutsceneRuins2.hpp"

#include <bn_sound_item.h>

#include "asset/Path.hpp"
#include "asset/SfxKind.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/TaskManager.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

static constexpr int BTN_ON_GFXIDX = 1;

CutsceneRuins2::CutsceneRuins2(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<CutsceneRuins2>(), isEnabled, triggers)
{
}

void CutsceneRuins2::awake(GameContext& ctx)
{
    if (ctx.state.getPlot() >= game::GamePlot::STEPPING_TILE_PUZZLE_COMPLETE)
    {
        auto* toriel = ctx.entMngr.findById(ent::gen::EntityId::toriel);
        BN_ASSERT(toriel);

        auto* button = ctx.entMngr.findById(ent::gen::EntityId::button);
        BN_ASSERT(button);
        auto* buttonSpr = button->getComponent<cpnt::Sprite>();
        BN_ASSERT(buttonSpr);

        auto* door = ctx.entMngr.findById(ent::gen::EntityId::door);
        BN_ASSERT(door);

        buttonSpr->setGfxIdx(BTN_ON_GFXIDX);

        toriel->setDestroyed(true);
        door->setDestroyed(true);
        _entity.setDestroyed(true);
    }
}

auto CutsceneRuins2::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    using namespace ut::asset;

    ctx.interactStack.push(InteractState::CUTSCENE);

    ctx.msgSettings = core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
    ctx.msgSettings.emotion = 2;
    ctx.isDialogUpper = false; // Force lower dialog box, otherwise Toriel is hidden

    ctx.state.setPlot(GamePlot::STEPPING_TILE_PUZZLE_COMPLETE);

    auto* toriel = ctx.entMngr.findById(ent::gen::EntityId::toriel);
    BN_ASSERT(toriel);
    auto* torielInput = toriel->getComponent<cpnt::NpcInput>();
    BN_ASSERT(torielInput);
    auto* torielWalk = toriel->getComponent<cpnt::WalkAnimCtrl>();
    BN_ASSERT(torielWalk);

    auto* button = ctx.entMngr.findById(ent::gen::EntityId::button);
    BN_ASSERT(button);
    auto* buttonSpr = button->getComponent<cpnt::Sprite>();
    BN_ASSERT(buttonSpr);

    auto* door = ctx.entMngr.findById(ent::gen::EntityId::door);
    BN_ASSERT(door);

    // 1. Toriel: talk
    task::SignalAwaiter talkAwaiter({task::TaskSignal::Kind::DIALOG_END});

    ctx.msg.clear();
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_306));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_307));
    ctx.game.startDialog();
    co_await talkAwaiter;

    // 2-1. Toriel: walk to the switch
    task::SignalAwaiter torielWalkAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});
    static constexpr bn::fixed MOVE_SPEED = 2.4;
    torielInput->startPath(IPath::get(gen::PathId::path_torielwalk2), MOVE_SPEED);
    co_await torielWalkAwaiter;

    // 2-2. Toriel: flip the switch -> open the door
    torielWalk->setStandStillDir(core::Directions::UP);
    getSfx(SfxKind::SWITCH_TRIGGER)->play();
    buttonSpr->setGfxIdx(BTN_ON_GFXIDX);
    door->setDestroyed(true);
    co_await task::TimeAwaiter(20);

    // 2-3. Toriel: walk to Frisk
    torielInput->startPath(IPath::get(gen::PathId::path_torielwalk2_2), MOVE_SPEED);
    co_await torielWalkAwaiter;

    // 3. Toriel: talk
    ctx.isDialogUpper = false; // Force lower dialog box, otherwise Toriel is hidden
    ctx.msg.clear();
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_311));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_312));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_313));
    ctx.msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_314));
    ctx.game.startDialog();
    co_await talkAwaiter;

    ctx.interactStack.pop();
    _entity.setDestroyed(true);
    ctx.msg.clear();
    ctx.msgSettings.reset();
    co_return;
}

} // namespace ut::game::cpnt::inter
