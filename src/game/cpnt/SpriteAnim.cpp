#include "game/cpnt/SpriteAnim.hpp"

#include <bn_sprite_item.h>

#include "asset/SpriteAnimInfo.hpp"
#include "asset/SpriteAnimKind.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/WalkAnimCtrl.hpp"
#include "game/ent/Entity.hpp"

namespace ut::game::cpnt
{

SpriteAnim::SpriteAnim(ent::Entity& entity, bool isEnabled, cpnt::Sprite& sprCpnt)
    : Component(entity, bn::type_id<SpriteAnim>(), isEnabled), _sprCpnt(sprCpnt),
      _curAnimKind(asset::SpriteAnimKind::NONE)
{
}

void SpriteAnim::render(GameContext&)
{
    if (!_isManualRender)
        renderOnce();
}

void SpriteAnim::renderOnce()
{
    if (_action.has_value() && !_action->done())
    {
        _action->update();

        if (_walkAnimCtrl != nullptr)
            _walkAnimCtrl->renderOnce();
    }
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
