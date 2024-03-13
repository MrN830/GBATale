#include "scene/IngameBattle.hpp"

#include "game/bt/state/BattleStateType.hpp"

using namespace ut::game::bt::state;

namespace ut::scene
{

IngameBattle::IngameBattle(SceneStack& sceneStack, SceneContext& context)
    : Scene(sceneStack, context, SceneId::INGAME_BATTLE)
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
    const auto nextState = _btState->update();

    if (nextState == BattleStateType::END_BATTLE)
        reqStackPop();
    else if (nextState != BattleStateType::NONE)
        changeBattleState(nextState);

    return false;
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
