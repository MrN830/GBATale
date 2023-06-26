#pragma once

#include "game/menu/MenuState.hpp"

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace ut::game::menu
{

class ItemMenu final : public MenuState
{
private:
    enum class State
    {
        SELECT_ITEM, // selecting which item to use
        SELECT_USAGE // selecting use/info/drop
    };

    enum Usage
    {
        USE,
        INFO,
        DROP
    };

public:
    ItemMenu(scene::IngameMenu&);

    auto handleInput() -> MenuStateType override;
    auto update() -> MenuStateType override;

private:
    auto handleItemSelect() -> MenuStateType;
    auto handleUsageSelect() -> MenuStateType;

    void moveCursorItem(bool playSfx);
    void moveCursorUsage(bool playSfx);

private:
    const int _itemCount;
    bn::vector<bn::sprite_ptr, 5 * 8 + 3> _text;

    State _itemMenuState = State::SELECT_ITEM;
    int _itemIdx = 0;
    int _usageIdx = 0;
};

} // namespace ut::game::menu
