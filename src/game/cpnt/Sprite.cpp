#include "game/cpnt/Sprite.hpp"

#include <bn_camera_ptr.h>
#include <bn_sprite_item.h>

#include "game/ent/Entity.hpp"

namespace ut::game::cpnt
{

Sprite::Sprite(ent::Entity& entity, const bn::sprite_item& sprItem, int gfxIdx, const bn::camera_ptr* camera,
               bool autoAlterZOrder)
    : Component(entity), _autoAlterZOrder(autoAlterZOrder), _spr(sprItem.create_sprite(entity.getPosition(), gfxIdx))
{
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

    if (_autoAlterZOrder)
        _spr.set_z_order(-absPos.y().floor_integer());
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

auto Sprite::getSprPtr() const -> const bn::sprite_ptr&
{
    return _spr;
}

auto Sprite::getSprPtr() -> bn::sprite_ptr&
{
    return _spr;
}

} // namespace ut::game::cpnt
