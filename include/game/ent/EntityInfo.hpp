#pragma once

#include <bn_fixed_point.h>
#include <bn_optional.h>
#include <bn_span.h>

#include "game/coll/CollInfo.hpp"

namespace bn
{
class sprite_item;
}

namespace ut::asset
{
enum class SpriteAnimKind : int16_t;
enum class WalkAnimKind : int8_t;
} // namespace ut::asset

namespace ut::game
{
struct GameContext;
}

namespace ut::game::cpnt::inter
{
enum class InteractionTriggers : uint8_t;
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
    struct ColliderPack
    {
        bn::span<const coll::RectCollInfo> staticCollInfos;
        bool isEnabled : 1;
        bool isTrigger : 1;
    };

    struct Interaction
    {
        bn::type_id_t type;
        cpnt::inter::InteractionTriggers triggers;
        bool isEnabled : 1;
    };

    struct Sprite
    {
        const bn::sprite_item& sprItem;
        uint16_t gfxIdx;
        int16_t zOrder;
        uint8_t bgPriority : 2;
        bool isEnabled : 1;
        bool isMoving : 1;
    };

    struct SpriteAnim
    {
        asset::SpriteAnimKind kind;
        bool isEnabled : 1;
    };

    struct WalkAnimCtrl
    {
        asset::WalkAnimKind kind;
        bool isEnabled : 1;
    };

public:
    gen::EntityId id;
    bn::fixed_point position;

    bn::optional<ColliderPack> collPack;
    bn::optional<Interaction> interaction;
    bn::optional<Sprite> sprite;
    bn::optional<SpriteAnim> sprAnim;
    bn::optional<WalkAnimCtrl> walkAnimCtrl;
};

} // namespace ut::game::ent
