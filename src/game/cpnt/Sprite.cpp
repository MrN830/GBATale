#include "game/cpnt/Sprite.hpp"

#include <bn_camera_ptr.h>
#include <bn_sprite_item.h>

#include "game/ent/Entity.hpp"

namespace ut::game::cpnt
{

Sprite::Sprite(ent::Entity& entity, const bn::sprite_item& sprItem, int gfxIdx, const bn::camera_ptr* camera,
               bool autoAlterZOrder, int bgPriority, int zOrder)
    : Component(entity), _updateZOrderOnMove(autoAlterZOrder), _sprItem(&sprItem),
      _spr(sprItem.create_sprite(entity.getPosition() + _diff, gfxIdx))
{
    _spr.set_blending_enabled(true);
    _spr.set_bg_priority(bgPriority);

    if (zOrder == Z_ORDER_UNSPECIFIED)
        updateZOrder();
    else
        _spr.set_z_order(zOrder);

    if (camera != nullptr)
        _spr.set_camera(*camera);
}

auto Sprite::getType() const -> bn::type_id_t
{
    return bn::type_id<Sprite>();
}

void Sprite::render(GameContext&)
{
    const auto absPos = _entity.getPosition() + _diff;
    _spr.set_position(absPos);

    if (_updateZOrderOnMove)
        updateZOrder();
}

void Sprite::onEntityActiveChanged(bool isEntityActive)
{
    _spr.set_visible(isEntityActive && isEnabled());
}

void Sprite::setEnabled(bool isEnabled)
{
    Component::setEnabled(isEnabled);

    _spr.set_visible(_entity.isActive() && isEnabled);
}

void Sprite::setDiff(const bn::fixed_point& diff)
{
    _diff = diff;
}

void Sprite::setSprItem(const bn::sprite_item& sprItem, int gfxIdx)
{
    _sprItem = &sprItem;
    _spr.set_item(sprItem, gfxIdx);
}

void Sprite::setGfxIdx(int gfxIdx)
{
    _spr.set_tiles(_sprItem->tiles_item(), gfxIdx);
}

auto Sprite::getSprPtr() const -> const bn::sprite_ptr&
{
    return _spr;
}

auto Sprite::getSprPtr() -> bn::sprite_ptr&
{
    return _spr;
}

void Sprite::updateZOrder()
{
    const auto absPos = _entity.getPosition() + _diff;

    _spr.set_z_order(-absPos.y().floor_integer());
}

} // namespace ut::game::cpnt
