#pragma once

#include "scene/Scene.hpp"

#include <bn_regular_bg_ptr.h>

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "core/Dialog.hpp"
#include "core/DialogWriter.hpp"

#include "consts.hpp"

namespace ut::scene::test
{

class TextChoiceTest : public Scene
{
public:
    TextChoiceTest(SceneStack&, SceneContext&);

    bool handleInput() override;
    bool update() override;

private:
    void redrawTextDataId();

private:
    bn::regular_bg_ptr _bg;

    core::DialogWriter _dialogWriter;

    bn::vector<core::Dialog, 190> _dialogs;
    bn::vector<bn::sprite_ptr, consts::DIALOG_MAX_SPRITES> _text;

private:
    int _curDialogIdx = 0;
    bn::vector<bn::sprite_ptr, 7> _textDataIdSpr;
};

} // namespace ut::scene::test
