#pragma once

#include <cstdint>

namespace ut::game::bt::mob
{

enum class MonsterAnimKind : uint8_t
{
    STOP, // if `once` anim is `done()`

    IDLE,
    HURT,
    KILLED,
    OTHER_1,
    OTHER_2,
};

class MonsterAnim
{
public:
    virtual ~MonsterAnim() = 0;

    virtual void render() = 0;

    void start(MonsterAnimKind);

    auto getAnimKind() const -> MonsterAnimKind;

protected:
    virtual void startAnim(MonsterAnimKind) = 0;

private:
    MonsterAnimKind _animKind = MonsterAnimKind::IDLE;
};

} // namespace ut::game::bt::mob
