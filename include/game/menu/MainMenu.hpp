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

    auto handleInput() -> MenuStateType override;
    auto update() -> MenuStateType override;

private:
    void moveCursor(bool playSfx);
};

} // namespace ut::game::menu
