#pragma once

#include "game/bt/state/BattleState.hpp"

namespace ut::game::bt::state
{

class BattleMenu final : public BattleState
{
public:
    BattleMenu(scene::IngameBattle&);

    auto handleInput() -> BattleStateType override;
    auto update() -> BattleStateType override;

private:
};

} // namespace ut::game::bt::state
