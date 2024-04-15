#pragma once

#include <bn_array.h>
#include <bn_fixed_point.h>
#include <bn_span.h>

namespace ut::asset
{

namespace gen
{
enum class PathId;
}

struct IPath
{
public:
    static auto get(gen::PathId) -> const IPath&;

public:
    constexpr virtual ~IPath() = default;

    virtual auto getPoints() const -> bn::span<const bn::fixed_point> = 0;
};

template <int PointCnt>
struct Path : IPath
{
public:
    constexpr Path(const bn::array<bn::fixed_point, PointCnt>& points_) : points(points_)
    {
    }

public:
    auto getPoints() const -> bn::span<const bn::fixed_point> override
    {
        return points;
    }

public:
    bn::array<bn::fixed_point, PointCnt> points;
};

} // namespace ut::asset
