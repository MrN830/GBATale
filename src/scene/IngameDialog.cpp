#include "scene/IngameDialog.hpp"

#include <bn_keypad.h>
#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "core/ChoiceMsgKind.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/cpnt/inter/PlotSpike.hpp"
#include "game/cpnt/inter/Readable.hpp"
#include "game/sys/EntityManager.hpp"

#include "gen/EntityId.hpp"
#include "gen/TextData.hpp"

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
    ctx.leftChoiceMsg = core::ChoiceMsgKind::NONE;
    ctx.rightChoiceMsg = core::ChoiceMsgKind::NONE;
}

bool IngameDialog::handleInput()
{
    if (bn::keypad::a_pressed())
    {
        auto result = _dialogWriter.confirmKeyInput();

        if (result != core::DialogWriter::TextChoice::NONE)
            resetToChoiceMsg(result);
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

void IngameDialog::resetToChoiceMsg(core::DialogWriter::TextChoice choice)
{
    auto* ctx = getContext().gameContext;
    BN_ASSERT(ctx != nullptr);

    using Choice = core::DialogWriter::TextChoice;
    using CMKind = core::ChoiceMsgKind;

    BN_ASSERT(choice != Choice::NONE);
    const auto choiceMsg = (choice == Choice::LEFT ? ctx->leftChoiceMsg : ctx->rightChoiceMsg);
    BN_ASSERT(choiceMsg != CMKind::NONE);

    ctx->msg.clear();

    auto& state = ctx->state;
    auto& flags = state.getFlags();
    const auto room = state.getRoom();
    const auto plot = state.getPlot();

    switch (choiceMsg)
    {
        using namespace ut::asset;

    case CMKind::CLOSE_IMMEDIATELY:
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1835));
        break;

    case CMKind::TAKE_CANDY_YES: {
        auto& items = state.getItems();

        auto dropCandyDish = [ctx]() {
            using namespace ut::game::cpnt::inter;

            auto* candyDish = ctx->entMngr.findById(game::ent::gen::EntityId::candy_dish);
            BN_ASSERT(candyDish != nullptr);
            auto* cdInter = candyDish->getComponent<Interaction>();
            BN_ASSERT(cdInter != nullptr);
            BN_ASSERT(cdInter->getInteractionType() == bn::type_id<Readable>());
            auto& readable = static_cast<Readable&>(*cdInter);

            readable.dropCandyDish(*ctx);
        };

        if (items.full())
            ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1635));
        else
        {
            items.push_back(game::ItemKind::MONSTER_CANDY);
            flags.candy_taken += 1;

            if (flags.candy_taken == 1)
                ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1624));
            else if (flags.candy_taken == 2)
                ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1625));
            else if (flags.candy_taken == 3)
            {
                if (flags.hardmode)
                {
                    flags.candy_taken += 1;
                    dropCandyDish();
                    ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1631));
                }
                else
                    ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1626));
            }
            else if (flags.candy_taken == 4)
            {
                dropCandyDish();
                ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1628));
            }
            else
                BN_ERROR("Invalid flags.candy_taken=", (int)flags.candy_taken);
        }
        break;
    }
    case CMKind::TAKE_CANDY_NO:
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1639));
        break;

    case CMKind::YELLOW_NAME_HELPFUL:
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4486));
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4487));
        break;
    case CMKind::YELLOW_NAME_BAD:
        ctx->leftChoiceMsg = CMKind::KEEP_YELLOW_NAMES;
        ctx->rightChoiceMsg = CMKind::NO_MORE_YELLOW_NAMES;
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4491));
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4492));
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4493));
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4494));
        break;

    case CMKind::KEEP_YELLOW_NAMES:
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4501));
        break;
    case CMKind::NO_MORE_YELLOW_NAMES:
        flags.name_color = game::GameFlags::NameColor::WHITE;
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4505));
        break;

    case CMKind::NO_NAME_COLOR_GREAT:
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4519));
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4520));
        break;
    case CMKind::BRING_NAME_COLOR_BACK:
        flags.name_color = game::GameFlags::NameColor::PINK;
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4524));
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4525));
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4526));
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4527));
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4528));
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_4529));
        break;

    case CMKind::TORIEL_DIARY_YES:
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1830));
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1831));
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1832));
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1833));
        break;

    case CMKind::PRESS_COLOR_SWITCH_OPEN: {
        using Room = game::RoomKind;
        using Plot = game::GamePlot;

        flags.ruins_switches_pressed += 1;
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1806));

        auto hideAllSpikesInRoom = [ctx]() {
            auto it = ctx->entMngr.beforeBeginIter();
            do
            {
                it = ctx->entMngr.findIf(
                    [](const game::ent::Entity& entity) -> bool {
                        const auto* inter = entity.getComponent<game::cpnt::inter::Interaction>();
                        return (inter != nullptr &&
                                inter->getInteractionType() == bn::type_id<game::cpnt::inter::PlotSpike>());
                    },
                    it);

                if (it != ctx->entMngr.endIter())
                {
                    auto* inter = it->getComponent<game::cpnt::inter::Interaction>();
                    BN_ASSERT(inter != nullptr);
                    BN_ASSERT(inter->getInteractionType() == bn::type_id<game::cpnt::inter::PlotSpike>());
                    auto* plotSpike = static_cast<game::cpnt::inter::PlotSpike*>(inter);

                    plotSpike->hideSpike();
                }

            } while (it != ctx->entMngr.endIter());
        };

        if (room == Room::ROOM_RUINS15B && (int)plot < (int)Plot::BLUE_SWITCH_FLIPPED)
        {
            state.setPlot(Plot::BLUE_SWITCH_FLIPPED);

            asset::getSfx(asset::SfxKind::SWITCH_PULL_N)->play();
            hideAllSpikesInRoom();
        }
        else if (room == Room::ROOM_RUINS15C && (int)plot < (int)Plot::RED_SWITCH_FLIPPED)
        {
            state.setPlot(Plot::RED_SWITCH_FLIPPED);

            asset::getSfx(asset::SfxKind::SWITCH_PULL_N)->play();
            hideAllSpikesInRoom();
        }
        else if (room == Room::ROOM_RUINS15D && (int)plot < (int)Plot::GREEN_SWITCH_FLIPPED)
        {
            state.setPlot(Plot::GREEN_SWITCH_FLIPPED);

            asset::getSfx(asset::SfxKind::SWITCH_PULL_N)->play();
            hideAllSpikesInRoom();
        }
        break;
    }
    case CMKind::PRESS_COLOR_SWITCH_NOTHING_HAPPENED:
        flags.ruins_switches_pressed += 1;

        if (flags.ruins_switches_pressed > 25)
            ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1790));
        else
            ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1788));
        break;
    case CMKind::PRESS_COLOR_SWITCH_TOO_MANY_TIMES:
        ctx->msg.push_back(gen::getTextEn(gen::TextData::SCR_TEXT_1790));
        break;

    default:
        BN_ERROR("Invalid ChoiceMsgKind=", (int)choiceMsg);
    }

    start();
}

} // namespace ut::scene
