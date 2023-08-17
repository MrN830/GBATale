#include "scene/IngameDialog.hpp"

#include <bn_display.h>
#include <bn_format.h>
#include <bn_keypad.h>

#include "asset/FontKind.hpp"
#include "core/DialogChoice.hpp"
#include "core/TextGens.hpp"
#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/sys/TaskManager.hpp"

#include "bn_regular_bg_items_bg_ingame_dialog.h"

namespace ut::scene
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = -bn::fixed_point{bn::display::width() / 2, bn::display::height() / 2};
constexpr auto GOLD_POS = bn::fixed_point{169, 72} + TOP_LEFT_ORIGIN;
constexpr auto ITEM_POS = bn::fixed_point{169, 87} + TOP_LEFT_ORIGIN;

enum BgMapIdx
{
    MAIN_L,
    MAIN_U,
    GOLD_L,
    GOLD_U,
};

} // namespace

static int getBgMapIdx(const game::GameContext* ctx)
{
    BN_ASSERT(ctx != nullptr);

    int mapIdx = (ctx->isDialogGold ? GOLD_L : MAIN_L);
    mapIdx = (int)mapIdx + (ctx->isDialogUpper ? 1 : 0);

    return mapIdx;
}

IngameDialog::IngameDialog(SceneStack& sceneStack, SceneContext& context)
    : Scene(sceneStack, context, SceneId::INGAME_DIALOG),
      _bg(bn::regular_bg_items::bg_ingame_dialog.create_bg(0, 0, getBgMapIdx(context.gameContext))),
      _dialogWriter(context.textGens, consts::INGAME_MENU_BG_PRIORITY)
{
    _bg.set_priority(consts::INGAME_MENU_BG_PRIORITY);

    start();
}

bool IngameDialog::handleInput()
{
    if (bn::keypad::a_pressed())
    {
        const int prevDialogIdx = _dialogWriter.getCurDialogIdx();
        auto result = _dialogWriter.confirmKeyInput();
        const int curDialogIdx = _dialogWriter.getCurDialogIdx();

        BN_ASSERT(getContext().gameContext != nullptr);
        auto& ctx = *getContext().gameContext;

        if (curDialogIdx > prevDialogIdx)
            ctx.taskMngr.onSignal({game::task::TaskSignal::Kind::DIALOG_INDEX, curDialogIdx});

        if (result != core::DialogChoice::NONE)
        {
            ctx.taskMngr.onSignal({game::task::TaskSignal::Kind::DIALOG_CHOICE, (int)result});

            if (_isDialogGold)
                redrawGoldDisplay();
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
        reset();

        auto* ctx = getContext().gameContext;
        BN_ASSERT(ctx != nullptr);

        ctx->taskMngr.onSignal({game::task::TaskSignal::Kind::DIALOG_END});
    }

    return true;
}

void IngameDialog::start()
{
    auto* ctx = getContext().gameContext;
    BN_ASSERT(ctx != nullptr);

    _bg.set_map(bn::regular_bg_items::bg_ingame_dialog.map_item(), getBgMapIdx(ctx));

    _isDialogGold = ctx->isDialogGold;
    _goldText.clear();
    if (_isDialogGold)
        redrawGoldDisplay();

    BN_ASSERT(!ctx->msg.empty(), "IngameDialog with empty gameContext.msg");
    using DSKind = core::Dialog::Settings::Kind;

    _dialogs.clear();
    const auto dialogSettings = (ctx->isDialogUpper ? DSKind::WORLD_UPPER : DSKind::WORLD_LOWER);
    for (const auto& str : ctx->msg)
        _dialogs.push_back(core::Dialog{dialogSettings, str});

    _dialogWriter.start(bn::span(_dialogs.cbegin(), _dialogs.cend()), _text);
}

void IngameDialog::reset()
{
    BN_ASSERT(getContext().gameContext != nullptr);
    auto& ctx = *getContext().gameContext;

    BN_ASSERT(ctx.interactStack.top() == game::InteractState::DIALOG);
    ctx.interactStack.pop();

    ctx.msg.clear();
}

void IngameDialog::redrawGoldDisplay()
{
    _goldText.clear();

    BN_ASSERT(getContext().gameContext != nullptr);
    const auto& state = getContext().gameContext->state;

    auto& textGen = getContext().textGens.get(asset::FontKind::MAIN);

    textGen.generate(GOLD_POS, bn::format<13>("$-{}G", state.getGold()), _goldText);
    textGen.generate(ITEM_POS, bn::format<9>("SPACE-{}/{}", state.getItems().size(), state.getItems().max_size()),
                     _goldText);

    for (auto& spr : _goldText)
        spr.set_bg_priority(consts::INGAME_MENU_BG_PRIORITY);
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
