#include "scene/IngameBattle.hpp"

#include "asset/Bgm.hpp"
#include "game/bt/BattleContext.hpp"
#include "game/bt/BattleGroup.hpp"
#include "game/bt/state/BattleStateType.hpp"

#include "game/bt/bt_consts.hpp"

using namespace ut::game::bt::state;

namespace ut::scene
{

IngameBattle::IngameBattle(SceneStack& sceneStack, SceneContext& context)
    : Scene(sceneStack, context, SceneId::INGAME_BATTLE),
      _movingBgBox(game::bt::consts::BG_BOX_INIT_RECT, game::bt::consts::BG_BOX_PRIORITY), _bg(BattleBgKind::NORMAL),
      _topUI(context.gameState, context.textGens), _mobDmgPopup(context.textGens),
      _monsterManager(context.battleContext)
{
    // initial dialog
    const auto& battleGroup = game::bt::BattleGroup::get(context.battleContext.battleGroupId);
    _btTempVars.dialogs[0].push_back(asset::gen::getTextEn(battleGroup.textData));

    asset::Bgm::play(battleGroup.bgm);

    changeBattleState(BattleStateType::BATTLE_MENU, false);
}

IngameBattle::~IngameBattle()
{
    getState().~BattleState();
}

bool IngameBattle::handleInput()
{
    const auto nextState = getState().handleInput();

    if (nextState == BattleStateType::END_BATTLE)
        reqStackPop();
    else if (nextState != BattleStateType::NONE)
        changeBattleState(nextState);

    return false;
}

bool IngameBattle::update()
{
    _movingBgBox.update();
    _atkBg.update();
    _monsterManager.update();

    const auto nextState = getState().update();

    if (nextState == BattleStateType::END_BATTLE)
        reqStackPop();
    else if (nextState != BattleStateType::NONE)
        changeBattleState(nextState);

    _monsterManager.render();
    _mobDmgPopup.render();
    _timedVfxManager.render();

    return false;
}

auto IngameBattle::getContext() -> SceneContext&
{
    return Scene::getContext();
}

auto IngameBattle::getContext() const -> const SceneContext&
{
    return Scene::getContext();
}

auto IngameBattle::getMovingBgBox() -> core::MovingBgBox&
{
    return _movingBgBox;
}

auto IngameBattle::getTopUI() -> game::bt::BattleTopUI&
{
    return _topUI;
}

auto IngameBattle::getAttackBg() -> game::bt::BattleAttackBg&
{
    return _atkBg;
}

auto IngameBattle::getMobDamagePopup() -> game::bt::MobDamagePopup&
{
    return _mobDmgPopup;
}

auto IngameBattle::getTimedVfxManager() -> game::bt::vfx::BattleTimedVfxManager&
{
    return _timedVfxManager;
}

auto IngameBattle::getMonsterManager() -> game::bt::MonsterManager&
{
    return _monsterManager;
}

auto IngameBattle::getMonsterManager() const -> const game::bt::MonsterManager&
{
    return _monsterManager;
}

auto IngameBattle::getBtTempVars() -> game::bt::BattleTempVars&
{
    return _btTempVars;
}

auto IngameBattle::getBtTempVars() const -> const game::bt::BattleTempVars&
{
    return _btTempVars;
}

void IngameBattle::changeBattleState(BattleStateType btStateType, bool hasPrevState)
{
    if (hasPrevState)
        getState().~BattleState();

    switch (btStateType)
    {
    case BattleStateType::BATTLE_MENU:
        ::new (static_cast<void*>(_btStateBuffer)) BattleMenu(*this);
        break;
    case BattleStateType::BATTLE_ATTACK:
        ::new (static_cast<void*>(_btStateBuffer)) BattleAttack(*this);
        break;
    case BattleStateType::BATTLE_PREPARE_DODGE:
        ::new (static_cast<void*>(_btStateBuffer)) BattlePrepareDodge(*this);
        break;
    case BattleStateType::BATTLE_DODGE:
        ::new (static_cast<void*>(_btStateBuffer)) BattleDodge(*this);
        break;
    case BattleStateType::BATTLE_END_DODGE:
        ::new (static_cast<void*>(_btStateBuffer)) BattleEndDodge(*this);
        break;

    default:
        BN_ERROR("Invalid btStateType=", (int)btStateType);
    }
}

auto IngameBattle::getState() -> game::bt::state::BattleState&
{
    return *reinterpret_cast<game::bt::state::BattleState*>(_btStateBuffer);
}

auto IngameBattle::getState() const -> const game::bt::state::BattleState&
{
    return *reinterpret_cast<const game::bt::state::BattleState*>(_btStateBuffer);
}

} // namespace ut::scene
