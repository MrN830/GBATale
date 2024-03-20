#include "game/bt/mob/Monster.hpp"

#include "game/bt/mob/MonsterInfo.hpp"
#include "game/bt/mob/MonsterKind.hpp"

namespace ut::game::bt::mob
{

Monster::Monster(MonsterKind kind, const bn::fixed_point& position) : _info(MonsterInfo::get(kind)), _position(position)
{
    switch (kind)
    {
    case MonsterKind::WHIMSUN:
        new (_animBuffer) WhimsunAnim(position);
        break;

    default:
        BN_ERROR("Invalid monsterKind=", (int)kind);
    }
}

Monster::~Monster()
{
    getAnim().~MonsterAnim();
}

void Monster::update()
{
}

void Monster::render()
{
    getAnim().render();
}

auto Monster::getInfo() const -> const MonsterInfo&
{
    return _info;
}

auto Monster::getPosition() const -> const bn::fixed_point&
{
    return _position;
}

auto Monster::getAnim() const -> const MonsterAnim&
{
    return *reinterpret_cast<const MonsterAnim*>(_animBuffer);
}

auto Monster::getAnim() -> MonsterAnim&
{
    return *reinterpret_cast<MonsterAnim*>(_animBuffer);
}

} // namespace ut::game::bt::mob
