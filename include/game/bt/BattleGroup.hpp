#pragma once

#include <cstdint>

#include <bn_array.h>
#include <bn_fixed_point.h>

#include "asset/gen/TextData_fwd.hpp"

namespace ut::asset
{
enum class BgmKind : uint8_t;
};

namespace ut::game::bt::mob
{
enum class MonsterKind : uint8_t;
}

namespace ut::game::bt
{

struct BattleGroup
{
public:
    static auto get(uint8_t id) -> const BattleGroup&;

public:
    uint8_t id;
    bn::array<mob::MonsterKind, 3> monsters;
    bn::array<bn::fixed_point, 3> monsterPositions;
    asset::gen::TextData textData;
    asset::BgmKind bgm;
};

} // namespace ut::game::bt
