#include "game/bt/mob/MonsterAnim.hpp"

namespace ut::game::bt::mob
{

MonsterAnim::~MonsterAnim() = default;

void MonsterAnim::start(MonsterAnimKind newAnimKind)
{
    startAnim(newAnimKind);

    _animKind = newAnimKind;
}

auto MonsterAnim::getAnimKind() const -> MonsterAnimKind
{
    return _animKind;
}

} // namespace ut::game::bt::mob
