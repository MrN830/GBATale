#pragma once

#include "game/menu/MenuState.hpp"

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace ut::game::menu
{

class StatMenu final : public MenuState
{
public:
    ~StatMenu();
    StatMenu(scene::IngameMenu&);

    auto handleInput() -> MenuStateType override;
    auto update() -> MenuStateType override;

private:
    bn::vector<bn::sprite_ptr, 40> _text;
};

} // namespace ut::game::menu
