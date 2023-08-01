#pragma once

#include "game/menu/MenuState.hpp"

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "core/Dialog.hpp"
#include "core/DialogWriter.hpp"

#include "consts.hpp"

namespace ut::game::menu
{

class DialogMenu final : public MenuState
{
public:
    static constexpr int DIALOGS = 7;

public:
    DialogMenu(scene::IngameMenu&);

    auto handleInput() -> MenuStateType override;
    auto update() -> MenuStateType override;

private:
    core::DialogWriter _dialogWriter;

    bn::vector<core::Dialog, DIALOGS> _dialogs;
    bn::vector<bn::sprite_ptr, consts::DIALOG_MAX_SPRITES> _text;
};

} // namespace ut::game::menu
