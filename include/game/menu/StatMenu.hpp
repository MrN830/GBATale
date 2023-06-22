#pragma once

#include "game/menu/MenuState.hpp"

namespace ut::game::menu
{

class StatMenu final : public MenuState
{
public:
    StatMenu(scene::IngameMenu&);

    auto handleInput(scene::IngameMenu&) -> MenuStateType override;
    auto update(scene::IngameMenu&) -> MenuStateType override;
};

} // namespace ut::game::menu
