#include "scene/IngameDialog.hpp"

#include <bn_keypad.h>

#include "core/DialogChoice.hpp"
#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/sys/TaskManager.hpp"

#include "bn_regular_bg_items_bg_battle_dialog.h"

namespace ut::scene
{

namespace
{
constexpr auto UPPER_DIALOG_DIFF = bn::fixed_point{0, -100};
}

IngameDialog::IngameDialog(SceneStack& sceneStack, SceneContext& context)
    : Scene(sceneStack, context, SceneId::INGAME_DIALOG), _bg(bn::regular_bg_items::bg_battle_dialog.create_bg(0, 0)),
      _dialogWriter(context.textGens, consts::INGAME_MENU_BG_PRIORITY)
{
    BN_ASSERT(context.gameContext != nullptr);
    auto& ctx = *context.gameContext;

    _bg.set_priority(consts::INGAME_MENU_BG_PRIORITY);
    if (ctx.isDialogUpper)
        _bg.set_position(_bg.position() + UPPER_DIALOG_DIFF);

    start();
}

IngameDialog::~IngameDialog()
{
    BN_ASSERT(getContext().gameContext != nullptr);
    auto& ctx = *getContext().gameContext;

    BN_ASSERT(ctx.interactStack.top() == game::InteractState::INTERACT);
    if (!ctx.isSavePromptRequested)
        ctx.interactStack.pop();

    ctx.msg.clear();
}

bool IngameDialog::handleInput()
{
    if (bn::keypad::a_pressed())
    {
        auto result = _dialogWriter.confirmKeyInput();

        if (result != core::DialogChoice::NONE)
        {
            BN_ASSERT(getContext().gameContext != nullptr);
            auto& ctx = *getContext().gameContext;

            ctx.taskMngr.onSignal({game::task::TaskSignal::Kind::DIALOG_CHOICE, (int)result});
        }
    }
    if (bn::keypad::b_pressed())
    {
        if (_dialogWriter.instantWrite())
            getContext().gameState.getFlags().dialogues_skipped++;
    }

    return true;
}

bool IngameDialog::update()
{
    _dialogWriter.update();

    if (!_dialogWriter.isStarted())
    {
        reqStackPop();

        auto* ctx = getContext().gameContext;
        BN_ASSERT(ctx != nullptr);

        ctx->taskMngr.onSignal({game::task::TaskSignal::Kind::DIALOG_END});

        if (ctx->isSavePromptRequested)
            reqStackPush(SceneId::SAVE_PROMPT);
    }

    return true;
}

void IngameDialog::start()
{
    auto* ctx = getContext().gameContext;
    BN_ASSERT(ctx != nullptr);

    BN_ASSERT(!ctx->msg.empty(), "IngameDialog with empty gameContext.msg");
    using DSKind = core::Dialog::Settings::Kind;

    _dialogs.clear();
    const auto dialogSettings = (ctx->isDialogUpper ? DSKind::WORLD_UPPER : DSKind::WORLD_LOWER);
    for (const auto& str : ctx->msg)
        _dialogs.push_back(core::Dialog{dialogSettings, str});

    _dialogWriter.start(bn::span(_dialogs.cbegin(), _dialogs.cend()), _text);
}

auto IngameDialog::getWriter() const -> const core::DialogWriter&
{
    return _dialogWriter;
}

auto IngameDialog::getWriter() -> core::DialogWriter&
{
    return _dialogWriter;
}

auto IngameDialog::getDialogs() -> decltype((_dialogs))
{
    return _dialogs;
}

auto IngameDialog::getDialogs() const -> decltype((_dialogs))
{
    return _dialogs;
}

} // namespace ut::scene
