#include "game/bt/mob/Monster.hpp"

#include "game/bt/mob/MonsterInfo.hpp"
#include "game/bt/mob/MonsterKind.hpp"

namespace ut::game::bt::mob
{

Monster::Monster(MonsterKind kind, const bn::fixed_point& position)
    : _info(MonsterInfo::get(kind)), _position(position), _curHp(_info.maxHp), _isActive(true)
{
    switch (kind)
    {
    case MonsterKind::FROGGIT:
        new (_animBuffer) FroggitAnim(position);
        new (_reactBuffer) FroggitReact();
        break;
    case MonsterKind::WHIMSUN:
        new (_animBuffer) WhimsunAnim(position);
        new (_reactBuffer) WhimsunReact();
        break;
    case MonsterKind::MOLDSMAL_1:
        new (_animBuffer) MoldsmalAnim(position);
        new (_reactBuffer) MoldsmalReact();
        break;

    default:
        BN_ERROR("Invalid monsterKind=", (int)kind);
    }
}

Monster::~Monster()
{
    getReact().~MonsterReact();
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

int Monster::getCurHp() const
{
    return _curHp;
}

void Monster::setCurHp(int curHp)
{
    _curHp = curHp;
}

bool Monster::isActive() const
{
    return _isActive;
}

void Monster::setActive(bool isActive)
{
    _isActive = isActive;
}

auto Monster::getAnim() const -> const MonsterAnim&
{
    return *reinterpret_cast<const MonsterAnim*>(_animBuffer);
}

auto Monster::getAnim() -> MonsterAnim&
{
    return *reinterpret_cast<MonsterAnim*>(_animBuffer);
}

auto Monster::getReact() const -> const MonsterReact&
{
    return *reinterpret_cast<const MonsterReact*>(_reactBuffer);
}

auto Monster::getReact() -> MonsterReact&
{
    return *reinterpret_cast<MonsterReact*>(_reactBuffer);
}

} // namespace ut::game::bt::mob
