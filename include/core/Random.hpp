#pragma once

#include <bn_random.h>

namespace ut::core
{

class Random final : public bn::random
{
public:
    struct Seeds
    {
        const unsigned x, y, z;
    };

public:
    constexpr auto getSeeds() const -> Seeds
    {
        return {_x, _y, _z};
    }

    constexpr void setSeeds(const Seeds& seeds)
    {
        _x = seeds.x;
        _y = seeds.y;
        _z = seeds.z;
    }
};

} // namespace ut::core
