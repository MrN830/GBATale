#pragma once

#include "scene/Scene.hpp"

#include "core/MovingBgBox.hpp"
#include "game/bt/state/BattleStates.hpp"

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
    auto getMovingBgBox() -> core::MovingBgBox&;

private:
    void changeBattleState(game::bt::state::BattleStateType, bool hasPrevState = true);

private:
    alignas(game::bt::state::BT_ST_ALIGN_SIZE) uint8_t _btStateBuffer[game::bt::state::BT_ST_MAX_SIZE];
    game::bt::state::BattleState* const _btState = reinterpret_cast<game::bt::state::BattleState*>(_btStateBuffer);

    core::MovingBgBox _movingBgBox;
};

} // namespace ut::scene
