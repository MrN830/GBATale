#pragma once

#include <cstdint>

#include <bn_fixed_point.h>

#include "game/bt/mob/MonsterAnims.hpp"

namespace ut::game::bt::mob
{

enum class MonsterKind : uint8_t;
struct MonsterInfo;

class Monster final
{
public:
    Monster(MonsterKind, const bn::fixed_point& position);
    ~Monster();

    void update();
    void render();

public:
    auto getInfo() const -> const MonsterInfo&;
    auto getPosition() const -> const bn::fixed_point&;

private:
    auto getAnim() const -> const MonsterAnim&;
    auto getAnim() -> MonsterAnim&;

private:
    const MonsterInfo& _info;
    const bn::fixed_point _position;

    alignas(MOB_ANIM_ALIGN_SIZE) uint8_t _animBuffer[MOB_ANIM_MAX_SIZE];
};

} // namespace ut::game::bt::mob
