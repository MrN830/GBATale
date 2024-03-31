#pragma once

#include "game/bt/state/BattleState.hpp"

#include "game/bt/BattleAttackBar.hpp"

namespace ut::core
{
enum class Directions : uint8_t;
}

namespace ut::game::bt::state
{

// TODO: support multi-hit weapons
class BattleAttack final : public BattleState
{
public:
    BattleAttack(scene::IngameBattle&);

    auto handleInput() -> BattleStateType override;
    auto update() -> BattleStateType override;

private:
    auto updateOnWaitBarCreation() -> BattleStateType;
    auto updateOnBarMoving() -> BattleStateType;
    auto updateOnSliceOngoing() -> BattleStateType;
    auto updateOnMobDamageAnimOngoing() -> BattleStateType;

private:
    enum class State
    {
        WAIT_BAR_CREATION,
        BAR_MOVING,
        SLICE_ONGOING,
        MOB_DAMAGE_ANIM_ONGOING,
    };

private:
    State _state;
    int _countdown;

    const core::Directions _barMoveDir;
    BattleAttackBar _bar;
    BattleAttackBarMoveByAction _barMoveAction;

    bn::sprite_ptr _slice;
    bn::optional<bn::sprite_animate_action<6>> _sliceAnim;
};

} // namespace ut::game::bt::state
