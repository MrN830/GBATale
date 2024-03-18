#pragma once

#include <cstdint>

#include "asset/gen/TextData_fwd.hpp"

namespace ut::game::bt
{

enum class MonsterKind : uint8_t;

struct MonsterInfo
{
public:
    static auto get(MonsterKind) -> const MonsterInfo&;

public:
    auto getName() const -> const bn::string_view&;

public:
    MonsterKind kind;

    int maxHp;
    int atk, def;
    int exp, gold;

    asset::gen::TextData nameData;
};

} // namespace ut::game::bt
