#include "game/cpnt/inter/Chairiel.hpp"

#include "asset/Path.hpp"
#include "asset/SpriteAnimKind.hpp"
#include "core/DialogChoice.hpp"
#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/NpcInput.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/cpnt/inter/InteractionType.hpp"
#include "game/ent/Entity.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"
#include "scene/SceneContext.hpp"

#include "gen/EntityId.hpp"
#include "gen/PathId.hpp"
#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

namespace
{

enum ChairielGfxIdx
{
    EMPTY_CHAIR = 9
};

} // namespace

Chairiel::Chairiel(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, InteractionType::Chairiel, isEnabled, triggers)
{
    BN_ASSERT(_entity.getId() == ent::gen::EntityId::chairiel);
}

void Chairiel::awake(GameContext& ctx)
{
    const auto plot = ctx.state.getPlot();
    _isTorielSitting = (GamePlot::TORIEL_READING <= plot && plot <= GamePlot::TORIEL_STANDING_UP);

    _chairielSpr = _entity.getComponent<Sprite>();
    BN_ASSERT(_chairielSpr);

    if (!_isTorielSitting)
        _chairielSpr->setGfxIdx(ChairielGfxIdx::EMPTY_CHAIR);
}

auto Chairiel::onInteract(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    Interaction::onInteract(ctx);

    auto plot = ctx.state.getPlot();
    auto& flags = ctx.state.getFlags();

    task::SignalAwaiter dialogEndAwaiter({task::TaskSignal::Kind::DIALOG_END});
    task::DialogChoiceAwaiter dialogChoiceAwaiter;

    if (_isTorielSitting)
    {
        ctx.msg.getSettings() =
            core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);
        ctx.msg.clearMsg();

        if (plot == GamePlot::RECEIVED_SNAIL_FACT)
        {
            ctx.msg.add(gen::TextData::SCR_TEXT_1927);
            ctx.msg.add(gen::TextData::SCR_TEXT_1930);

            ctx.game.startDialog();
            const auto dialogChoice = co_await dialogChoiceAwaiter;

            ctx.msg.clearMsg();
            if (dialogChoice == core::DialogChoice::LEFT)
                ctx.msg.add(gen::TextData::SCR_TEXT_1934);
            else
            {
                ctx.msg.add(gen::TextData::SCR_TEXT_1935);
                ctx.state.setPlot(GamePlot::I_HAVE_TO_DO_SOMETHING);
            }
        }
        else if (plot == GamePlot::I_HAVE_ALWAYS_WANTED_TO_BE_A_TEACHER)
        {
            ctx.msg.add(gen::TextData::SCR_TEXT_1897);
            ctx.msg.add(gen::TextData::SCR_TEXT_1898);
        MSG_529:
            ctx.msg.add(gen::TextData::SCR_TEXT_1899);
            ctx.msg.add(gen::TextData::SCR_TEXT_1900);
            ctx.msg.add(gen::TextData::SCR_TEXT_1901);

            ctx.game.startDialog();
            auto dialogChoice = co_await dialogChoiceAwaiter;

            ctx.msg.clearMsg();
            if (dialogChoice == core::DialogChoice::LEFT)
                ctx.msg.add(gen::TextData::SCR_TEXT_1907);
            else
                ctx.msg.add(gen::TextData::SCR_TEXT_1908);
            ctx.msg.add(gen::TextData::SCR_TEXT_1909);

            const int r = ctx.sceneContext.rng.get_int(4);
            if (r == 0)
                ctx.msg.add(gen::TextData::SCR_TEXT_1911);
            else if (r == 1)
                ctx.msg.add(gen::TextData::SCR_TEXT_1912);
            else if (r == 2)
                ctx.msg.add(gen::TextData::SCR_TEXT_1913);
            else
                ctx.msg.add(gen::TextData::SCR_TEXT_1914);

            ctx.msg.add(gen::TextData::SCR_TEXT_1915);
            ctx.msg.add(gen::TextData::SCR_TEXT_1916);

            ctx.game.startDialog();
            dialogChoice = co_await dialogChoiceAwaiter;

            ctx.state.setPlot(GamePlot::RECEIVED_SNAIL_FACT);

            ctx.msg.clearMsg();
            if (dialogChoice == core::DialogChoice::LEFT)
                ctx.msg.add(gen::TextData::SCR_TEXT_1921);
            else
            {
                ctx.msg.add(gen::TextData::SCR_TEXT_1924);
                ctx.state.setPlot(GamePlot::I_HAVE_TO_DO_SOMETHING);
            }
        }
        else if (plot == GamePlot::RECEIVED_SLEEP_SUGGESTION || flags.got_bscotch_pie > GameFlags::GotBscotchPie::INIT)
        {
        MSG_528:
            if (flags.got_bscotch_pie > GameFlags::GotBscotchPie::INIT)
                ctx.msg.add(gen::TextData::SCR_TEXT_1871);
            else
                ctx.msg.add(gen::TextData::SCR_TEXT_1870);
            ctx.msg.add(gen::TextData::SCR_TEXT_1872);
            ctx.msg.add(gen::TextData::SCR_TEXT_1873);
            ctx.msg.add(gen::TextData::SCR_TEXT_1874);
            ctx.msg.add(gen::TextData::SCR_TEXT_1875);
            ctx.msg.add(gen::TextData::SCR_TEXT_1876);
            ctx.msg.add(gen::TextData::SCR_TEXT_1877);
            ctx.msg.add(gen::TextData::SCR_TEXT_1878);
            ctx.msg.add(gen::TextData::SCR_TEXT_1879);
            ctx.msg.add(gen::TextData::SCR_TEXT_1880);
            ctx.msg.add(gen::TextData::SCR_TEXT_1881);
            ctx.msg.add(gen::TextData::SCR_TEXT_1882);
            ctx.msg.add(gen::TextData::SCR_TEXT_1883);

            ctx.game.startDialog();
            const auto dialogChoice = co_await dialogChoiceAwaiter;

            ctx.state.setPlot(GamePlot::I_HAVE_ALWAYS_WANTED_TO_BE_A_TEACHER);

            ctx.msg.clearMsg();
            if (dialogChoice == core::DialogChoice::LEFT)
                ctx.msg.add(gen::TextData::SCR_TEXT_1891);
            else
            {
                ctx.msg.add(gen::TextData::SCR_TEXT_1895);
                ctx.msg.add(gen::TextData::SCR_TEXT_1896);
                goto MSG_529;
            }
        }
        else
        {
            ctx.msg.add(gen::TextData::SCR_TEXT_1859);
            ctx.msg.add(gen::TextData::SCR_TEXT_1860);
            ctx.msg.add(gen::TextData::SCR_TEXT_1861);
            ctx.msg.add(gen::TextData::SCR_TEXT_1862);

            ctx.game.startDialog();
            const auto dialogChoice = co_await dialogChoiceAwaiter;

            ctx.state.setPlot(GamePlot::RECEIVED_SLEEP_SUGGESTION);

            ctx.msg.clearMsg();
            if (dialogChoice == core::DialogChoice::LEFT)
                ctx.msg.add(gen::TextData::SCR_TEXT_1867);
            else
                goto MSG_528;
        }

        ctx.game.startDialog();
        co_await dialogEndAwaiter;

        ctx.msg.getSettings().reset();

        plot = ctx.state.getPlot();
        if (plot == GamePlot::I_HAVE_TO_DO_SOMETHING)
        {
            ctx.interactStack.push(InteractState::INTERACT);

            auto* chairielAnim = _entity.getComponent<SpriteAnim>();
            BN_ASSERT(chairielAnim);

            chairielAnim->setCurAnimKind(SpriteAnimKind::CHAIRIEL_GET_UP);
            co_await task::SignalAwaiter({task::TaskSignal::Kind::SPR_ANIM_END, (int)ent::gen::EntityId::chairiel});

            ctx.state.setPlot(GamePlot::TORIEL_IN_BASEMENT);
            _isTorielSitting = false;

            BN_ASSERT(ctx.interactStack.top() == InteractState::INTERACT);
            ctx.interactStack.pop();

            auto* toriel = ctx.entMngr.findById(ent::gen::EntityId::toriel);
            BN_ASSERT(toriel);
            auto* torielSpr = toriel->getComponent<Sprite>();
            BN_ASSERT(torielSpr);
            auto* torielWalk = toriel->getComponent<WalkAnimCtrl>();
            BN_ASSERT(torielWalk);
            auto* torielInput = toriel->getComponent<NpcInput>();
            BN_ASSERT(torielInput);

            _chairielSpr->setGfxIdx(ChairielGfxIdx::EMPTY_CHAIR);
            torielSpr->setEnabled(true);
            torielWalk->setStandStillDir(core::Directions::RIGHT);
            torielInput->startPath(IPath::get(gen::PathId::path_torielwalkhouse1), 4);
            co_await task::SignalAwaiter({task::TaskSignal::Kind::NPC_WALK_END, (int)ent::gen::EntityId::toriel});

            toriel->setDestroyed(true);
        }
    }
    else
    {
        ctx.msg.clearMsg();
        ctx.msg.add(gen::TextData::obj_chairiel_118);
        ctx.game.startDialog();
    }

    co_return;
}

} // namespace ut::game::cpnt::inter
