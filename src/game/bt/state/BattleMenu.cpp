#include "game/bt/state/BattleMenu.hpp"

#include "game/bt/state/BattleStateType.hpp"
#include "game/bt/state/sbm/BattleSubmenuType.hpp"

namespace ut::game::bt::state
{

BattleMenu::BattleMenu(scene::IngameBattle& scene) : BattleState(scene), _nextBtState(BattleStateType::NONE)
{
    changeSubmenu(sbm::BattleSubmenuType::MAIN, false);
}

BattleMenu::~BattleMenu()
{
    getSubmenu().~BattleSubmenu();
}

auto BattleMenu::handleInput() -> BattleStateType
{
    const auto nextSubmenu = getSubmenu().handleInput();

    if (_nextBtState != BattleStateType::NONE)
        return _nextBtState;
    else if (nextSubmenu != sbm::BattleSubmenuType::NONE)
        changeSubmenu(nextSubmenu);

    return BattleStateType::NONE;
}

auto BattleMenu::update() -> BattleStateType
{
    const auto nextSubmenu = getSubmenu().update();

    if (_nextBtState != BattleStateType::NONE)
        return _nextBtState;
    else if (nextSubmenu != sbm::BattleSubmenuType::NONE)
        changeSubmenu(nextSubmenu);

    return BattleStateType::NONE;
}

void BattleMenu::setNextBattleState(BattleStateType nextBtState)
{
    _nextBtState = nextBtState;
}

auto BattleMenu::getScene() -> scene::IngameBattle&
{
    return _scene;
}

auto BattleMenu::getScene() const -> const scene::IngameBattle&
{
    return _scene;
}

void BattleMenu::changeSubmenu(sbm::BattleSubmenuType submenuType, bool hasPrevSubmenu)
{
    if (hasPrevSubmenu)
        getSubmenu().~BattleSubmenu();

    switch (submenuType)
    {
    case sbm::BattleSubmenuType::MAIN:
        ::new (static_cast<void*>(_submenuBuffer)) sbm::BattleSubmenuMain(*this);
        break;
    case sbm::BattleSubmenuType::FIGHT:
        ::new (static_cast<void*>(_submenuBuffer)) sbm::BattleSubmenuFight(*this);
        break;
    // case sbm::BattleSubmenuType::ACT:
    //     ::new (static_cast<void*>(_submenuBuffer)) sbm::BattleSubmenuAct(*this);
    //     break;
    // case sbm::BattleSubmenuType::ITEM:
    //     ::new (static_cast<void*>(_submenuBuffer)) sbm::BattleSubmenuItem(*this);
    //     break;
    // case sbm::BattleSubmenuType::MERCY:
    //     ::new (static_cast<void*>(_submenuBuffer)) sbm::BattleSubmenuMercy(*this);
    //     break;
    default:
        BN_ERROR("Invalid submenuType=", (int)submenuType);
    }
}

auto BattleMenu::getSubmenu() const -> const sbm::BattleSubmenu&
{
    return *reinterpret_cast<const sbm::BattleSubmenu*>(_submenuBuffer);
}

auto BattleMenu::getSubmenu() -> sbm::BattleSubmenu&
{
    return *reinterpret_cast<sbm::BattleSubmenu*>(_submenuBuffer);
}

} // namespace ut::game::bt::state
