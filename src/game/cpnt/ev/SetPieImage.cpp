#include "game/cpnt/ev/SetPieImage.hpp"

#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/ent/Entity.hpp"

namespace ut::game::cpnt::ev
{

namespace
{

enum PieGfxIdx
{
    SMALL_PIECE = 0,
    BIG_FULL = 1,
    BIG_SLICED = 2,
};

} // namespace

SetPieImage::SetPieImage(ent::Entity& entity, bool isEnabled, bool isAutoFire)
    : EventComponent(entity, bn::type_id<SetPieImage>(), isEnabled, isAutoFire)
{
}

auto SetPieImage::onEvent(GameContext& ctx) -> task::Task
{
    const auto& flags = ctx.state.getFlags();

    auto* spr = _entity.getComponent<cpnt::Sprite>();
    BN_ASSERT(spr != nullptr);

    const int gfxIdx =
        (flags.got_bscotch_pie != GameFlags::GotBscotchPie::INIT) ? PieGfxIdx::BIG_SLICED : PieGfxIdx::BIG_FULL;
    spr->setGfxIdx(gfxIdx);

    co_return;
}

} // namespace ut::game::cpnt::ev
