#pragma once

#include "game/bt/state/BattleState.hpp"

#include "game/bt/state/sbm/BattleSubmenus.hpp"

namespace ut::game::bt::state
{

class BattleMenu final : public BattleState
{
public:
    BattleMenu(scene::IngameBattle&);
    ~BattleMenu();

    auto handleInput() -> BattleStateType override;
    auto update() -> BattleStateType override;

public:
    void setNextBattleState(BattleStateType);

public:
    auto getScene() -> scene::IngameBattle&;
    auto getScene() const -> const scene::IngameBattle&;

private:
    void changeSubmenu(sbm::BattleSubmenuType, bool hasPrevSubmenu = true);

private:
    auto getSubmenu() const -> const sbm::BattleSubmenu&;
    auto getSubmenu() -> sbm::BattleSubmenu&;

private:
    alignas(sbm::BT_SBM_ALIGN_SIZE) uint8_t _submenuBuffer[sbm::BT_SBM_MAX_SIZE];

    BattleStateType _nextBtState;
};

} // namespace ut::game::bt::state
