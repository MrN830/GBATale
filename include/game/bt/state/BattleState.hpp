#pragma once

namespace ut::scene
{
class IngameBattle;
};

namespace ut::game::bt::state
{

enum class BattleStateType;

class BattleState
{
public:
    virtual ~BattleState() = default;
    BattleState(scene::IngameBattle& scene) : _scene(scene){};

    /// @return next battle state
    [[nodiscard]] virtual auto handleInput() -> BattleStateType = 0;

    /// @return next battle state
    [[nodiscard]] virtual auto update() -> BattleStateType = 0;

protected:
    scene::IngameBattle& _scene;
};

} // namespace ut::game::bt::state
