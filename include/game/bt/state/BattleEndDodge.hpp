#pragma once

#include "game/bt/state/BattleState.hpp"

namespace ut::game::bt::state
{

class BattleEndDodge final : public BattleState
{
public:
    BattleEndDodge(scene::IngameBattle&);

    auto handleInput() -> BattleStateType override;
    auto update() -> BattleStateType override;

private:
};

} // namespace ut::game::bt::state
