#pragma once

#include "scene/Scene.hpp"

#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "game/menu/MenuStates.hpp"

namespace ut::scene
{

class IngameMenu : public Scene
{
    friend class game::menu::MainMenu;
    friend class game::menu::StatMenu;

public:
    IngameMenu(SceneStack&, Context&);

    bool handleInput() override;
    bool update() override;

private:
    void changeMenuState(game::menu::MenuStateType, bool hasPrevState = true);

private:
    const int _menuItemCount;

    alignas(game::menu::MENU_ALIGN_SIZE) uint8_t _menuStateBuffer[game::menu::MENU_MAX_SIZE];
    game::menu::MenuState* const _menuState = reinterpret_cast<game::menu::MenuState*>(_menuStateBuffer);

    bn::regular_bg_ptr _bg;
    bn::sprite_ptr _cursor;
    bn::vector<bn::sprite_ptr, 10> _text;
};

} // namespace ut::scene
