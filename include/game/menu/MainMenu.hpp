#pragma once

#include "game/menu/MenuState.hpp"

namespace bn
{
class sprite_ptr;
}

namespace ut::game::menu
{

class MainMenu final : public MenuState
{
public:
    MainMenu(scene::IngameMenu&);

    auto handleInput(scene::IngameMenu&) -> MenuStateType override;
    auto update(scene::IngameMenu&) -> MenuStateType override;

private:
    void moveCursor(bn::sprite_ptr&, scene::IngameMenu&);
};

} // namespace ut::game::menu
