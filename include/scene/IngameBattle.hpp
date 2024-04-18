#pragma once

#include "scene/Scene.hpp"

#include "core/MovingBgBox.hpp"
#include "game/bt/BattleAttackBg.hpp"
#include "game/bt/BattleBg.hpp"
#include "game/bt/BattleTempVars.hpp"
#include "game/bt/BattleTopUI.hpp"
#include "game/bt/MobDamagePopup.hpp"
#include "game/bt/MonsterManager.hpp"
#include "game/bt/state/BattleStates.hpp"
#include "game/bt/vfx/BattleTimedVfxManager.hpp"

namespace ut::scene
{

class IngameBattle : public Scene
{
public:
    IngameBattle(SceneStack&, SceneContext&);
    ~IngameBattle();

    bool handleInput() override;
    bool update() override;

public:
    auto getContext() -> SceneContext&;
    auto getContext() const -> const SceneContext&;

    auto getMovingBgBox() -> core::MovingBgBox&;
    auto getTopUI() -> game::bt::BattleTopUI&;
    auto getAttackBg() -> game::bt::BattleAttackBg&;

    auto getMobDamagePopup() -> game::bt::MobDamagePopup&;
    auto getTimedVfxManager() -> game::bt::vfx::BattleTimedVfxManager&;

    auto getMonsterManager() -> game::bt::MonsterManager&;
    auto getMonsterManager() const -> const game::bt::MonsterManager&;

    auto getBtTempVars() -> game::bt::BattleTempVars&;
    auto getBtTempVars() const -> const game::bt::BattleTempVars&;

private:
    void changeBattleState(game::bt::state::BattleStateType, bool hasPrevState = true);

private:
    auto getState() -> game::bt::state::BattleState&;
    auto getState() const -> const game::bt::state::BattleState&;

private:
    alignas(game::bt::state::BT_ST_ALIGN_SIZE) uint8_t _btStateBuffer[game::bt::state::BT_ST_MAX_SIZE];

    core::MovingBgBox _movingBgBox;
    game::bt::BattleBg _bg;
    game::bt::BattleTopUI _topUI;
    game::bt::BattleAttackBg _atkBg;

    game::bt::MobDamagePopup _mobDmgPopup;
    game::bt::vfx::BattleTimedVfxManager _timedVfxManager;

    game::bt::MonsterManager _monsterManager;

    game::bt::BattleTempVars _btTempVars;
};

} // namespace ut::scene
