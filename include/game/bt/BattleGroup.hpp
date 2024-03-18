#pragma once

#include <cstdint>

#include "asset/gen/TextData_fwd.hpp"

namespace ut::asset
{
enum class BgmKind : uint8_t;
};

namespace ut::game::bt
{

enum class MonsterKind : uint8_t;

struct BattleGroup
{
public:
    static auto get(uint8_t id) -> const BattleGroup&;

public:
    uint8_t id;
    MonsterKind monsters[3];
    asset::gen::TextData textData;
    asset::BgmKind bgm;
};

} // namespace ut::game::bt
