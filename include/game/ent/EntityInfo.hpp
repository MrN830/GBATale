#pragma once

#include <bn_fixed_point.h>
#include <bn_optional.h>

namespace bn
{
class sprite_item;
}

namespace ut::game
{
struct GameContext;
}

namespace ut::game::ent
{

namespace gen
{
enum class EntityId : uint16_t;
}

struct EntityInfo
{
public:
    void create(GameContext&) const;

public:
    struct Sprite
    {
        const bn::sprite_item& sprItem;
        uint16_t gfxIdx;
        int16_t zOrder;
        bool isEnabled : 1;
        bool isMoving : 1;
    };

public:
    gen::EntityId id;
    bn::fixed_point position;
    bn::optional<Sprite> sprite;
};

} // namespace ut::game::ent
