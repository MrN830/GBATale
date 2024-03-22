#pragma once

namespace ut::game::bt::state
{
class BattleMenu;
enum class BattleStateType;
} // namespace ut::game::bt::state

namespace ut::game::bt::state::sbm
{

enum class BattleSubmenuType;

class BattleSubmenu
{
public:
    virtual ~BattleSubmenu() = default;
    BattleSubmenu(BattleMenu& priMenu) : _priMenu(priMenu){};

    /// @return next battle submenu
    [[nodiscard]] virtual auto handleInput() -> BattleSubmenuType = 0;

protected:
    BattleMenu& _priMenu;
};

} // namespace ut::game::bt::state::sbm
