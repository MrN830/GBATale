#pragma once

#include "game/menu/MenuState.hpp"

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace ut::game::menu
{

class CellMenu final : public MenuState
{
public:
    CellMenu(scene::IngameMenu&);
    ~CellMenu();

    auto handleInput() -> MenuStateType override;
    auto update() -> MenuStateType override;

private:
    bn::vector<bn::sprite_ptr, 5 * 8> _text;
};

} // namespace ut::game::menu
