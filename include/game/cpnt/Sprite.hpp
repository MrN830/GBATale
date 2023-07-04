#pragma once

#include "game/cpnt/Component.hpp"

#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>

namespace bn
{
class camera_ptr;
}

namespace ut::game::cpnt
{

class Sprite : public Component
{
public:
    Sprite(ent::Entity&, const bn::sprite_item&, int gfxIdx, const bn::camera_ptr*, bool autoAlterZOrder);

    auto getType() const -> bn::type_id_t override;

    void render(GameContext&) override;

    void onEntityActiveChanged(bool isEntityActive) override;
    void setEnabled(bool isEnabled) override;

public:
    void setDiff(const bn::fixed_point& diff);

    auto getSprPtr() const -> const bn::sprite_ptr&;
    auto getSprPtr() -> bn::sprite_ptr&;

private:
    const bool _autoAlterZOrder;

    bn::sprite_ptr _spr;
    bn::fixed_point _diff;
};

} // namespace ut::game::cpnt
