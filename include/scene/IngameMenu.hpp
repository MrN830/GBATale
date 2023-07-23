#pragma once

#include "scene/Scene.hpp"

#include <bn_fixed_point.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>
#include <bn_vector.h>

#include "consts.hpp"
#include "game/menu/MenuStates.hpp"

namespace ut::game
{
enum class ItemKind : uint8_t;
}

namespace ut::scene
{

class IngameMenu : public Scene
{
public:
    static constexpr int BG_PRIORITY = consts::INGAME_MENU_BG_PRIORITY;

public:
    friend class game::menu::MainMenu;
    friend class game::menu::DialogMenu;
    friend class game::menu::ItemMenu;
    friend class game::menu::StatMenu;
    friend class game::menu::CellMenu;

private:
    static constexpr bn::fixed_point ZERO_DIFF = {0, 0};
    static constexpr bn::fixed_point STAT_LOWER_DIFF = {0, +116};
    static constexpr bn::fixed_point MENU_LOWER_DIFF = {0, +16};

public:
    IngameMenu(SceneStack&, SceneContext&);
    ~IngameMenu();

    bool handleInput() override;
    bool update() override;

public:
    bool isItemMenuEnabled() const;
    bool isCellMenuEnabled() const;
    bool isDialogUpper() const;

    void redrawTexts();

private:
    void changeMenuState(game::menu::MenuStateType, bool hasPrevState = true);

private:
    const int _menuItemCount;
    const bool _isDialogUpper;

    alignas(game::menu::MENU_ALIGN_SIZE) uint8_t _menuStateBuffer[game::menu::MENU_MAX_SIZE];
    game::menu::MenuState* const _menuState = reinterpret_cast<game::menu::MenuState*>(_menuStateBuffer);

    bn::regular_bg_ptr _bg;
    bn::sprite_ptr _cursor;
    bn::vector<bn::sprite_ptr, 12> _text;
    bn::vector<bn::string<consts::DIALOG_MAX_CHARS>, game::menu::DialogMenu::DIALOGS> _dialogs;
};

} // namespace ut::scene
