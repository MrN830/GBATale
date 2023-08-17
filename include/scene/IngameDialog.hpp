#pragma once

#include "scene/Scene.hpp"

#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "core/Dialog.hpp"
#include "core/DialogWriter.hpp"

#include "consts.hpp"

namespace ut::scene
{

class IngameDialog : public Scene
{
public:
    IngameDialog(SceneStack&, SceneContext&);

    bool handleInput() override;
    bool update() override;

public:
    void start();

private:
    void reset();

    void redrawGoldDisplay();

private:
    bn::regular_bg_ptr _bg;

    core::DialogWriter _dialogWriter;

    bn::vector<core::Dialog, consts::DIALOG_MAX_LINES> _dialogs;
    bn::vector<bn::sprite_ptr, consts::DIALOG_MAX_SPRITES> _text;

    bn::vector<bn::sprite_ptr, 7> _goldText;

    bool _isDialogGold = false;

public:
    auto getWriter() const -> const core::DialogWriter&;
    auto getWriter() -> core::DialogWriter&;

    auto getDialogs() -> decltype((_dialogs));
    auto getDialogs() const -> decltype((_dialogs));
};

} // namespace ut::scene
