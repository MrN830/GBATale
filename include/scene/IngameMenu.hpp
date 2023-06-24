#pragma once

#include "scene/Scene.hpp"

#include <bn_fixed_point.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "game/menu/MenuStates.hpp"

namespace ut::scene
{

class IngameMenu : public Scene
{
public:
    static constexpr int BG_PRIORITY = 2;

public:
    friend class game::menu::MainMenu;
    friend class game::menu::StatMenu;

private:
    static constexpr bn::fixed_point ZERO_DIFF = {0, 0};
    static constexpr bn::fixed_point STAT_LOWER_DIFF = {0, +116};
    static constexpr bn::fixed_point MENU_LOWER_DIFF = {0, +16};

public:
    IngameMenu(SceneStack&, Context&);

    bool handleInput() override;
    bool update() override;

public:
    bool isItemMenuEnabled() const;
    bool isCellMenuEnabled() const;
    bool isDialogUpper() const;

    void redrawItemMenuText(bool isItemMenuEnabled);

private:
    void changeMenuState(game::menu::MenuStateType, bool hasPrevState = true);

private:
    const int _menuItemCount;
    const bool _isItemMenuEnabled;
    const bool _isDialogUpper;

    alignas(game::menu::MENU_ALIGN_SIZE) uint8_t _menuStateBuffer[game::menu::MENU_MAX_SIZE];
    game::menu::MenuState* const _menuState = reinterpret_cast<game::menu::MenuState*>(_menuStateBuffer);

    bn::regular_bg_ptr _bg;
    bn::sprite_ptr _cursor;
    bn::vector<bn::sprite_ptr, 11> _text;
    bn::vector<bn::sprite_ptr, 1> _itemMenuText;
};

} // namespace ut::scene
