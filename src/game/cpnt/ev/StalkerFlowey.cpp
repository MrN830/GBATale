#include "game/cpnt/ev/StalkerFlowey.hpp"

#include <bn_display.h>
#include <bn_fixed_rect.h>

#include "asset/SpriteAnimKind.hpp"
#include "game/GameContext.hpp"
#include "game/cpnt/SpriteAnim.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/CameraManager.hpp"
#include "game/sys/TaskManager.hpp"
#include "gen/EntityId.hpp"

namespace ut::game::cpnt::ev
{

StalkerFlowey::StalkerFlowey(ent::Entity& entity, bool isEnabled, bool isAutoFire)
    : EventComponent(entity, bn::type_id<StalkerFlowey>(), isEnabled, isAutoFire)
{
    BN_ASSERT(entity.getId() == ent::gen::EntityId::flowey,
              "StalkerFlowey attached to non-flowey: ", (int)entity.getId());
}

void StalkerFlowey::update(GameContext& ctx)
{
    if (!_isHideStarted && isOnScreen(ctx))
    {
        _isHideStarted = true;

        auto task = onEvent(ctx);
        if (!task.done())
            ctx.taskMngr.addTask(std::move(task));
    }
}

auto StalkerFlowey::onEvent(GameContext&) -> task::Task
{
    auto* anim = _entity.getComponent<SpriteAnim>();
    BN_ASSERT(anim);
    anim->setCurAnimKind(asset::SpriteAnimKind::STALKER_FLOWEY);
    co_await task::SignalAwaiter({task::TaskSignal::Kind::SPR_ANIM_END, (int)_entity.getId()});

    _entity.setDestroyed(true);
    co_return;
}

bool StalkerFlowey::isOnScreen(GameContext& ctx) const
{
    static constexpr bn::fixed_size FLOWEY_SIZE = {17, 17};

    const auto& cam = ctx.camMngr.getCamera();

    const bn::fixed_rect camRect(cam.position(), bn::display::size());

    const bn::fixed_point floweyCenter(_entity.getPosition().x(), _entity.getPosition().y() - FLOWEY_SIZE.height() / 2);
    const bn::fixed_rect floweyRect(floweyCenter, FLOWEY_SIZE);

    return camRect.intersects(floweyRect);
}

} // namespace ut::game::cpnt::ev
