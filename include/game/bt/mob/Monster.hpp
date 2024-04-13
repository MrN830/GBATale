#pragma once

#include <cstdint>

#include <bn_fixed_point.h>

#include "game/bt/mob/MonsterAnims.hpp"
#include "game/bt/mob/MonsterReacts.hpp"

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
    void receiveDamage(int rawDamage);

public:
    auto getInfo() const -> const MonsterInfo&;
    auto getPosition() const -> const bn::fixed_point&;

    int getCurHp() const;
    void setCurHp(int curHp);

    // not spared, killed, bored
    bool isActive() const;
    void setActive(bool isActive);

public:
    auto getAnim() const -> const MonsterAnim&;
    auto getAnim() -> MonsterAnim&;

    auto getReact() const -> const MonsterReact&;
    auto getReact() -> MonsterReact&;

private:
    const MonsterInfo& _info;
    const bn::fixed_point _position;

    int _curHp;
    bool _isActive;

    alignas(MOB_ANIM_ALIGN_SIZE) uint8_t _animBuffer[MOB_ANIM_MAX_SIZE];
    alignas(MOB_REACT_ALIGN_SIZE) uint8_t _reactBuffer[MOB_REACT_MAX_SIZE];
};

} // namespace ut::game::bt::mob
