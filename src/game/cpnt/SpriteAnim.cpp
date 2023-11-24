#include "game/cpnt/SpriteAnim.hpp"

#include <bn_sprite_item.h>

#include "asset/SpriteAnimInfo.hpp"
#include "asset/SpriteAnimKind.hpp"
#include "game/GameContext.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/ent/Entity.hpp"
#include "game/sys/TaskManager.hpp"

namespace ut::game::cpnt
{

SpriteAnim::SpriteAnim(ent::Entity& entity, bool isEnabled, cpnt::Sprite& sprCpnt)
    : Component(entity, bn::type_id<SpriteAnim>(), isEnabled), _sprCpnt(sprCpnt),
      _curAnimKind(asset::SpriteAnimKind::NONE)
{
}

void SpriteAnim::render(GameContext& ctx)
{
    if (!_isManualRender)
        renderOnce(ctx);
}

void SpriteAnim::renderOnce(GameContext& ctx)
{
    if (_action.has_value() && !_action->done())
    {
        _action->update();

        if (_walkAnimCtrl != nullptr)
            _walkAnimCtrl->renderOnce();

        if (_action->done())
            _lastAnimFrameWait = _action->wait_updates() + 1 + 1;
    }

    if (_lastAnimFrameWait > 0 && --_lastAnimFrameWait == 0)
        ctx.taskMngr.onSignal({task::TaskSignal::Kind::SPR_ANIM_END, (int)_entity.getId()});
}

bool SpriteAnim::isDone() const
{
    return !_action.has_value() || (_action->done() && _lastAnimFrameWait <= 0);
}

bool SpriteAnim::isManualRender() const
{
    return _isManualRender;
}

auto SpriteAnim::getCurAnimKind() const -> asset::SpriteAnimKind
{
    return _curAnimKind;
}

/**
 * @brief Set animation kind, and restart the animation if necessary.
 */
void SpriteAnim::setCurAnimKind(asset::SpriteAnimKind kind)
{
    if (kind == _curAnimKind)
        return;

    _lastAnimFrameWait = -1;

    if (_walkAnimCtrl != nullptr)
        _walkAnimCtrl->setCurAnimKind(kind, false);
    else
    {
        _curAnimKind = kind;

        if (kind == asset::SpriteAnimKind::NONE)
        {
            _isManualRender = false;
            _action.reset();
        }
        else
        {
            const auto& info = asset::ISpriteAnimInfo::get(kind);

            _isManualRender = info.manualRender;

            auto& spr = _sprCpnt.getSprPtr();
            _sprCpnt.setDiff(info.diff);
            spr.set_item(info.sprItem);
            spr.set_horizontal_flip(info.hFlip);
            spr.set_vertical_flip(info.vFlip);

            using SprAnimAction = decltype(_action)::value_type;

            // resolve a func ptr by name to an *overloaded* function, kinda ugly
            using CreateAnimFunc = SprAnimAction (*)(const bn::sprite_ptr&, int, const bn::sprite_tiles_item&,
                                                     const bn::span<const uint16_t>&);
            auto* createAnim = (info.forever ? static_cast<CreateAnimFunc>(SprAnimAction::forever)
                                             : static_cast<CreateAnimFunc>(SprAnimAction::once));

            _action = createAnim(spr, info.waitUpdate, info.sprItem.tiles_item(), info.getGfxIdxes());
        }
    }
}

} // namespace ut::game::cpnt
