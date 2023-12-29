#include "game/cpnt/inter/TalkNpcArea1.hpp"

#include "core/MsgViewHolder.hpp"
#include "game/GameContext.hpp"
#include "game/GameFlags.hpp"
#include "game/GamePlot.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/ent/Entity.hpp"
#include "game/task/TaskAwaiters.hpp"
#include "scene/Game.hpp"

#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

TalkNpcArea1::TalkNpcArea1(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<TalkNpcArea1>(), isEnabled, triggers)
{
}

void TalkNpcArea1::awake(GameContext& ctx)
{
    auto* walk = _entity.getComponent<WalkAnimCtrl>();
    BN_ASSERT(walk);

    walk->setStandStillDir(core::Directions::LEFT);

    const auto plot = ctx.state.getPlot();
    const auto& flags = ctx.state.getFlags();

    if (flags.true_pacifist)
    {
        // TODO: Persist save `EndF` > 0
        if (false)
            _entity.setDestroyed(true);
        else
        {
            // TODO: Set Asriel sprite
        }
    }
    else if (plot != GamePlot::TORIEL_DEFEATED || flags.status_toriel == GameFlags::StatusToriel::KILLED)
    {
        _entity.setDestroyed(true);
    }
}

auto TalkNpcArea1::onInteract(GameContext& ctx) -> task::Task
{
    using namespace ut::asset;

    Interaction::onInteract(ctx);

    auto& flags = ctx.state.getFlags();

    ctx.msg.clearMsg();
    // Asriel talks
    if (flags.true_pacifist)
    {
        // TODO: Set Asriel dialog portrait

        switch (flags.conversation_asriel2)
        {
        case 0:
            ctx.msg.add(gen::TextData::obj_torinteractable7_93);
            ctx.msg.add(gen::TextData::obj_torinteractable7_94);
            break;
        case 1:
            ctx.msg.getSettings().emotion = 7;
            ctx.msg.add(gen::TextData::obj_torinteractable7_99);
            ctx.msg.add(gen::TextData::obj_torinteractable7_100);
            break;
        case 2:
            ctx.msg.getSettings().emotion = 7;
            ctx.msg.add(gen::TextData::obj_torinteractable7_106);
            ctx.msg.add(gen::TextData::obj_torinteractable7_107);
            break;
        case 3:
            ctx.msg.getSettings().emotion = 7;
            ctx.msg.add(gen::TextData::obj_torinteractable7_113);
            ctx.msg.add(gen::TextData::obj_torinteractable7_114);
            ctx.msg.add(gen::TextData::obj_torinteractable7_115);
            ctx.msg.add(gen::TextData::obj_torinteractable7_116);
            ctx.msg.add(gen::TextData::obj_torinteractable7_117);
            ctx.msg.add(gen::TextData::obj_torinteractable7_118);
            ctx.msg.add(gen::TextData::obj_torinteractable7_119);
            ctx.msg.add(gen::TextData::obj_torinteractable7_120);
            ctx.msg.add(gen::TextData::obj_torinteractable7_121);
            ctx.msg.add(gen::TextData::obj_torinteractable7_122);
            ctx.msg.add(gen::TextData::obj_torinteractable7_123);
            ctx.msg.add(gen::TextData::obj_torinteractable7_124);
            ctx.msg.add(gen::TextData::obj_torinteractable7_125);
            ctx.msg.add(gen::TextData::obj_torinteractable7_126);
            ctx.msg.add(gen::TextData::obj_torinteractable7_127);
            ctx.msg.add(gen::TextData::obj_torinteractable7_128);
            ctx.msg.add(gen::TextData::obj_torinteractable7_129);
            break;
        case 4:
            ctx.msg.getSettings().emotion = 2;
            ctx.msg.add(gen::TextData::obj_torinteractable7_135);
            ctx.msg.add(gen::TextData::obj_torinteractable7_136);
            ctx.msg.add(gen::TextData::obj_torinteractable7_137);
            ctx.msg.add(gen::TextData::obj_torinteractable7_138);
            ctx.msg.add(gen::TextData::obj_torinteractable7_139);
            ctx.msg.add(gen::TextData::obj_torinteractable7_140);
            break;
        case 5:
            ctx.msg.getSettings().emotion = 2;
            ctx.msg.add(gen::TextData::obj_torinteractable7_146);
            ctx.msg.add(gen::TextData::obj_torinteractable7_147);
            ctx.msg.add(gen::TextData::obj_torinteractable7_148);
            ctx.msg.add(gen::TextData::obj_torinteractable7_149);
            ctx.msg.add(gen::TextData::obj_torinteractable7_150);
            ctx.msg.add(gen::TextData::obj_torinteractable7_151);
            ctx.msg.add(gen::TextData::obj_torinteractable7_152);
            ctx.msg.add(gen::TextData::obj_torinteractable7_153);
            ctx.msg.add(gen::TextData::obj_torinteractable7_154);
            break;
        case 6:
            ctx.msg.getSettings().emotion = 2;
            ctx.msg.add(gen::TextData::obj_torinteractable7_160);
            ctx.msg.add(gen::TextData::obj_torinteractable7_161);
            ctx.msg.add(gen::TextData::obj_torinteractable7_162);
            ctx.msg.add(gen::TextData::obj_torinteractable7_163);
            ctx.msg.add(gen::TextData::obj_torinteractable7_164);
            ctx.msg.add(gen::TextData::obj_torinteractable7_165);
            ctx.msg.add(gen::TextData::obj_torinteractable7_166);
            ctx.msg.add(gen::TextData::obj_torinteractable7_167);
            ctx.msg.add(gen::TextData::obj_torinteractable7_168);
            ctx.msg.add(gen::TextData::obj_torinteractable7_169);
            ctx.msg.add(gen::TextData::obj_torinteractable7_170);
            ctx.msg.add(gen::TextData::obj_torinteractable7_171);
            ctx.msg.add(gen::TextData::obj_torinteractable7_172);
            ctx.msg.add(gen::TextData::obj_torinteractable7_173);
            ctx.msg.add(gen::TextData::obj_torinteractable7_174);
            ctx.msg.add(gen::TextData::obj_torinteractable7_175);
            ctx.msg.add(gen::TextData::obj_torinteractable7_176);
            ctx.msg.add(gen::TextData::obj_torinteractable7_177);
            ctx.msg.add(gen::TextData::obj_torinteractable7_178);
            ctx.msg.add(gen::TextData::obj_torinteractable7_179);
            ctx.msg.add(gen::TextData::obj_torinteractable7_180);
            ctx.msg.add(gen::TextData::obj_torinteractable7_181);
            ctx.msg.add(gen::TextData::obj_torinteractable7_182);
            ctx.msg.add(gen::TextData::obj_torinteractable7_183);
            ctx.msg.add(gen::TextData::obj_torinteractable7_184);
            break;
        case 7:
            ctx.msg.add(gen::TextData::obj_torinteractable7_190);
            ctx.msg.add(gen::TextData::obj_torinteractable7_191);
            ctx.msg.add(gen::TextData::obj_torinteractable7_192);
            ctx.msg.add(gen::TextData::obj_torinteractable7_193);
            ctx.msg.add(gen::TextData::obj_torinteractable7_194);
            ctx.msg.add(gen::TextData::obj_torinteractable7_195);
            ctx.msg.add(gen::TextData::obj_torinteractable7_196);
            ctx.msg.add(gen::TextData::obj_torinteractable7_197);
            ctx.msg.add(gen::TextData::obj_torinteractable7_198);
            ctx.msg.add(gen::TextData::obj_torinteractable7_199);
            ctx.msg.add(gen::TextData::obj_torinteractable7_200);
            ctx.msg.add(gen::TextData::obj_torinteractable7_201);
            ctx.msg.add(gen::TextData::obj_torinteractable7_202);
            ctx.msg.add(gen::TextData::obj_torinteractable7_203);
            ctx.msg.add(gen::TextData::obj_torinteractable7_204);
            ctx.msg.add(gen::TextData::obj_torinteractable7_205);
            break;
        default:
            ctx.msg.add(gen::TextData::obj_torinteractable7_212);
            ctx.msg.add(gen::TextData::obj_torinteractable7_213);
            break;
        }

        flags.conversation_asriel2 += 1;
    }
    // Toriel talks
    else
    {
        ctx.msg.getSettings() =
            core::DialogSettingsOverride::getPreset(core::DialogSettingsOverride::PresetKind::WORLD_TORIEL);

        ctx.msg.add(gen::TextData::obj_torinteractable7_82);
        ctx.msg.add(gen::TextData::obj_torinteractable7_83);
    }

    ctx.game.startDialog();
    co_await task::SignalAwaiter({task::TaskSignal::Kind::DIALOG_END});

    ctx.msg.getSettings().reset();
    co_return;
}

} // namespace ut::game::cpnt::inter
