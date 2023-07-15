#pragma once

#include "game/cpnt/Component.hpp"

#include <bn_fixed_point.h>
#include <bn_sprite_ptr.h>

#include "consts.hpp"

namespace bn
{
class camera_ptr;
}

namespace ut::game::cpnt
{

class Sprite : public Component
{
public:
    static constexpr int Z_ORDER_UNSPECIFIED = -32768;

public:
    Sprite(ent::Entity&, const bn::sprite_item&, int gfxIdx, const bn::camera_ptr*, bool updateZOrderOnMove,
           int bgPriority = consts::WORLD_BG_PRIORITY, int zOrder = Z_ORDER_UNSPECIFIED);

    auto getType() const -> bn::type_id_t override;

    void render(GameContext&) override;

    void onEntityActiveChanged(bool isEntityActive) override;
    void setEnabled(bool isEnabled) override;

public:
    void setDiff(const bn::fixed_point& diff);

    void setSprItem(const bn::sprite_item&, int gfxIdx);
    void setGfxIdx(int gfxIdx);

    auto getSprPtr() const -> const bn::sprite_ptr&;
    auto getSprPtr() -> bn::sprite_ptr&;

private:
    void updateZOrder();

private:
    const bool _updateZOrderOnMove;
    const bn::sprite_item* _sprItem;

    bn::sprite_ptr _spr;
    bn::fixed_point _diff;
};

} // namespace ut::game::cpnt
