#include "game/cpnt/ev/CutsceneTorhouse3.hpp"

#include "asset/Path.hpp"
#include "asset/SpriteAnimKind.hpp"
#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/sys/EntityManager.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::ev
{

CutsceneTorhouse3::CutsceneTorhouse3(ent::Entity& entity, bool isEnabled, bool isAutoFire)
    : EventComponent(entity, bn::type_id<CutsceneTorhouse3>(), isEnabled, isAutoFire)
{
    BN_ASSERT(entity.getId() == ent::gen::EntityId::toriel);
}

void CutsceneTorhouse3::awake(GameContext& ctx)
{
    if (ctx.state.getPlot() > GamePlot::TORIEL_WAITING_AT_BEDROOMS)
        _entity.setDestroyed(true);
}

auto CutsceneTorhouse3::onEvent(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    auto* torielSprAnim = _entity.getComponent<SpriteAnim>();
    BN_ASSERT(torielSprAnim);
    auto* torielInput = _entity.getComponent<NpcInput>();
    BN_ASSERT(torielInput);
    auto* torielWalk = _entity.getComponent<WalkAnimCtrl>();
    BN_ASSERT(torielWalk);

    auto* frisk = ctx.entMngr.findById(ent::gen::EntityId::frisk);
    BN_ASSERT(frisk);
    auto* friskSpr = frisk->getComponent<Sprite>();
    BN_ASSERT(friskSpr);
    auto* friskInput = frisk->getComponent<NpcInput>();
    BN_ASSERT(friskInput);
    auto* friskWalk = frisk->getComponent<WalkAnimCtrl>();
    BN_ASSERT(friskWalk);

    task::SignalAwaiter dialogEndAwaiter({task::TaskSignal::Kind::DIALOG_END});
    task::SignalAwaiter torielWalkAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});
    task::SignalAwaiter torielSprAnimAwaiter({task::TaskSignal::Kind::SPR_ANIM_END, (int)ent::gen::EntityId::toriel});

    ctx.interactStack.push(InteractState::CUTSCENE);

    friskWalk->setStandStillDir(core::Directions::RIGHT);
    torielWalk->setStandStillDir(core::Directions::LEFT);

    // Toriel talks
    ctx.msg.getSettings() =
        core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::obj_torieltrigger7_41);
    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    // Toriel & Frisk walk in front of Asriel's room
    friskSpr->setEnabled(false);
    const auto& pathRoomFront = IPath::get(gen::PathId::path_torielwalk20_2);
    torielWalk->registerWalkAnimKind(WalkAnimKind::TORIEL_HANDHOLD_FRISK);
    torielWalk->setStandStillDir(core::Directions::RIGHT);
    torielInput->startPath(pathRoomFront, 1.6);
    co_await torielWalkAwaiter;

    // Toriel & Frisk look at the Asriel's room
    torielWalk->registerWalkAnimKind(WalkAnimKind::TORIEL);
    const auto roomFrontPos = pathRoomFront.getPoints().back();
    _entity.setPosition(roomFrontPos + bn::fixed_point(9, 2));
    frisk->setPosition(roomFrontPos + bn::fixed_point(-11, 0));
    friskSpr->setEnabled(true);
    torielWalk->setStandStillDir(core::Directions::UP);
    friskWalk->setStandStillDir(core::Directions::UP);

    // Toriel talks
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::obj_torieltrigger7_243);
    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    // Toriel ruffles Frisk's hair
    constexpr bn::fixed_point RUFFLE_DIFF = {-9, -2};
    friskSpr->setEnabled(false);
    torielWalk->setEnabled(false);
    torielInput->setEnabled(false);
    torielSprAnim->setCurAnimKind(SpriteAnimKind::TORIEL_RUFFLE_FRISK_HAIR);
    _entity.setPosition(_entity.getPosition() + RUFFLE_DIFF);
    co_await torielSprAnimAwaiter;

    // Toriel looks back
    friskSpr->setEnabled(true);
    torielWalk->setEnabled(true);
    torielInput->setEnabled(true);
    torielWalk->setStandStillDir(core::Directions::DOWN);
    _entity.setPosition(_entity.getPosition() - RUFFLE_DIFF + bn::fixed_point(0, -1));

    // Toriel talks
    ctx.msg.getSettings().emotion = 1;
    ctx.msg.clearMsg();
    ctx.msg.add(gen::TextData::obj_torieltrigger7_77);
    ctx.game.startDialog();
    co_await dialogEndAwaiter;

    ctx.msg.getSettings().reset();
    ctx.state.setPlot(GamePlot::TORIEL_READING);
    BN_ASSERT(ctx.interactStack.top() == InteractState::CUTSCENE);
    ctx.interactStack.pop();

    // Toriel goes out of room
    torielInput->startPath(IPath::get(gen::PathId::path_torielwalk20), 4.8);
    co_await torielWalkAwaiter;

    _entity.setDestroyed(true);
    co_return;
}

} // namespace ut::game::cpnt::ev
