#pragma once

#include <bn_fixed_point.h>
#include <bn_fixed_size.h>
#include <bn_type_id.h>

#include "core/Directions.hpp"

namespace bn
{
class fixed_size;
}

namespace ut::game::coll
{

struct CollInfo;
struct RectCollInfo;
struct CircleCollInfo;
struct AAIRTriCollInfo;

struct RectCollInfo final
{
    bn::fixed_point position;
    bn::fixed_size size;

    bool isCollideWith(const CollInfo& other) const;
    bool isCollideWith(const RectCollInfo& other) const;
    bool isCollideWith(const CircleCollInfo& other) const;
    bool isCollideWith(const AAIRTriCollInfo& other) const;
};

struct CircleCollInfo final
{
    bn::fixed_point position;
    bn::fixed radius;

    bool isCollideWith(const CollInfo& other) const;
    bool isCollideWith(const RectCollInfo& other) const;
    bool isCollideWith(const CircleCollInfo& other) const;
    bool isCollideWith(const AAIRTriCollInfo& other) const;
};

/**
 * @brief Axis-Aligned Isosceles Right Triangle collision info.
 */
struct AAIRTriCollInfo final
{
    bn::fixed_point position; // right angle vertex position
    bn::fixed legLength;
    core::Directions directions; // hypotenuse -> right angle direction

    bool isCollideWith(const CollInfo& other) const;
    bool isCollideWith(const RectCollInfo& other) const;
    bool isCollideWith(const CircleCollInfo& other) const;
    bool isCollideWith(const AAIRTriCollInfo& other) const;
};

/**
 * @brief Type-erased collision info.
 */
struct CollInfo final
{
public:
    const bn::type_id_t type;
    union {
        RectCollInfo rect;
        CircleCollInfo circle;
        AAIRTriCollInfo tri;
    };

public:
    bool isCollideWith(const CollInfo& other) const;
    bool isCollideWith(const RectCollInfo& other) const;
    bool isCollideWith(const CircleCollInfo& other) const;
    bool isCollideWith(const AAIRTriCollInfo& other) const;

public:
    /**
     * @brief Create rect collider info.
     */
    constexpr explicit CollInfo(const RectCollInfo& info) : type(bn::type_id<RectCollInfo>()), rect(info)
    {
    }

    /**
     * @brief Create circle collider info.
     */
    constexpr explicit CollInfo(const CircleCollInfo& info) : type(bn::type_id<CircleCollInfo>()), circle(info)
    {
    }

    constexpr explicit CollInfo(const AAIRTriCollInfo& info) : type(bn::type_id<AAIRTriCollInfo>()), tri(info)
    {
    }
};

}; // namespace ut::game::coll
