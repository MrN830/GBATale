#include "scene/IngameDialog.hpp"

#include <bn_keypad.h>

#include "game/GameContext.hpp"

#include "bn_regular_bg_items_bg_battle_dialog.h"

namespace ut::scene
{

namespace
{
constexpr auto UPPER_DIALOG_DIFF = bn::fixed_point{0, -100};
}

IngameDialog::IngameDialog(SceneStack& sceneStack, SceneContext& context)
    : Scene(sceneStack, context), _bg(bn::regular_bg_items::bg_battle_dialog.create_bg(0, 0)),
      _dialogWriter(context.textGens, consts::INGAME_MENU_BG_PRIORITY)
{
    BN_ASSERT(context.gameContext != nullptr);
    auto& ctx = *context.gameContext;

    _bg.set_priority(consts::INGAME_MENU_BG_PRIORITY);
    if (ctx.isDialogUpper)
        _bg.set_position(_bg.position() + UPPER_DIALOG_DIFF);

    BN_ASSERT(!ctx.msg.empty(), "IngameDialog with empty gameContext.msg");
    using DSKind = core::Dialog::Settings::Kind;
    const auto dialogSettings = (ctx.isDialogUpper ? DSKind::WORLD_UPPER : DSKind::WORLD_LOWER);
    for (const auto& str : ctx.msg)
        _dialogs.push_back(core::Dialog{dialogSettings, str});

    _dialogWriter.start(bn::span(_dialogs.cbegin(), _dialogs.cend()), _text);
}

IngameDialog::~IngameDialog()
{
    BN_ASSERT(getContext().gameContext != nullptr);
    auto& ctx = *getContext().gameContext;

    BN_ASSERT(ctx.interactStack.top() == game::InteractState::INTERACT);
    ctx.interactStack.pop();

    ctx.msg.clear();
}

bool IngameDialog::handleInput()
{
    if (bn::keypad::a_pressed())
        _dialogWriter.keyInput();
    if (bn::keypad::b_pressed())
        _dialogWriter.instantWrite();

    return false; // Stop handling input in game world
}

bool IngameDialog::update()
{
    _dialogWriter.update();

    if (!_dialogWriter.isStarted())
        reqStackPop();

    return true;
}

} // namespace ut::scene
