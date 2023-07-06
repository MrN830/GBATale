#pragma once

#include <bn_fixed_point.h>
#include <bn_fixed_size.h>
#include <bn_type_id.h>

namespace ut::game::coll
{

struct CollInfo
{
public:
    CollInfo(const bn::fixed_point& position_);
    virtual ~CollInfo() = default;
    virtual auto getType() const -> bn::type_id_t = 0;

public:
    bool isCollideWith(const CollInfo& other) const;

public:
    bn::fixed_point position;
};

struct RectCollInfo : public CollInfo
{
public:
    RectCollInfo(const bn::fixed_point& position_, const bn::fixed_size size_);
    auto getType() const -> bn::type_id_t override;

public:
    bn::fixed_size size;
};

struct CircleCollInfo : public CollInfo
{
public:
    CircleCollInfo(const bn::fixed_point& position_, const bn::fixed radius_);
    auto getType() const -> bn::type_id_t override;

public:
    bn::fixed radius;
};

inline constexpr int INFO_ALIGN_SIZE = bn::max({alignof(RectCollInfo), alignof(CircleCollInfo)});
inline constexpr int INFO_MAX_SIZE = bn::max({sizeof(RectCollInfo), sizeof(CircleCollInfo)});

}; // namespace ut::game::coll
