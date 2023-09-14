#include "game/cpnt/inter/RuinsColorSwitchHelp.hpp"

#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/ent/Entity.hpp"
#include "scene/Game.hpp"

#include "gen/TextData.hpp"

namespace ut::game::cpnt::inter
{

RuinsColorSwitchHelp::RuinsColorSwitchHelp(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<RuinsColorSwitchHelp>(), isEnabled, triggers)
{
}

void RuinsColorSwitchHelp::awake(GameContext& ctx)
{
    const auto& flags = ctx.state.getFlags();

    if (flags.wrong_switches_pressed != 2)
        _entity.setDestroyed(true);
}

auto RuinsColorSwitchHelp::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    auto& flags = ctx.state.getFlags();

    ctx.msg.clear();
    ctx.msg.push_back(asset::gen::getTextEn(asset::gen::TextData::obj_switchhelptrigger_69));
    ctx.game.startDialog();

    flags.wrong_switches_pressed += 1;
    _entity.setDestroyed(true);

    co_return;
}

} // namespace ut::game::cpnt::inter
