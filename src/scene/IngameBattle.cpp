#include "scene/IngameBattle.hpp"

#include "game/bt/state/BattleStateType.hpp"

#include "game/bt/bt_consts.hpp"

using namespace ut::game::bt::state;

namespace ut::scene
{

IngameBattle::IngameBattle(SceneStack& sceneStack, SceneContext& context)
    : Scene(sceneStack, context, SceneId::INGAME_BATTLE),
      _movingBgBox(game::bt::consts::BG_BOX_INIT_RECT, game::bt::consts::BG_BOX_PRIORITY), _bg(BattleBgKind::NORMAL), _monsterManager(context.battleContext)
{
    changeBattleState(BattleStateType::BATTLE_MENU, false);
}

IngameBattle::~IngameBattle()
{
    _btState->~BattleState();
}

bool IngameBattle::handleInput()
{
    const auto nextState = _btState->handleInput();

    if (nextState == BattleStateType::END_BATTLE)
        reqStackPop();
    else if (nextState != BattleStateType::NONE)
        changeBattleState(nextState);

    return false;
}

bool IngameBattle::update()
{
    _movingBgBox.update();
    _monsterManager.update();

    const auto nextState = _btState->update();

    if (nextState == BattleStateType::END_BATTLE)
        reqStackPop();
    else if (nextState != BattleStateType::NONE)
        changeBattleState(nextState);

    _monsterManager.render();

    return false;
}

auto IngameBattle::getMovingBgBox() -> core::MovingBgBox&
{
    return _movingBgBox;
}

void IngameBattle::changeBattleState(BattleStateType btStateType, bool hasPrevState)
{
    if (hasPrevState)
        _btState->~BattleState();

    switch (btStateType)
    {
    case BattleStateType::BATTLE_MENU:
        new (_btState) BattleMenu(*this);
        break;
    case BattleStateType::BATTLE_ATTACK:
        new (_btState) BattleAttack(*this);
        break;
    case BattleStateType::BATTLE_PREPARE_DODGE:
        new (_btState) BattlePrepareDodge(*this);
        break;
    case BattleStateType::BATTLE_DODGE:
        new (_btState) BattleDodge(*this);
        break;
    case BattleStateType::BATTLE_END_DODGE:
        new (_btState) BattleEndDodge(*this);
        break;

    default:
        BN_ERROR("Invalid btStateType=", (int)btStateType);
    }
}

} // namespace ut::scene
