#pragma once

#include <cstdint>

namespace ut::scene
{
class IngameBattle;
}

namespace ut::game::bt::mob
{

enum class MonsterKind : uint8_t;

class Monster;

class MonsterReact
{
public:
    virtual ~MonsterReact() = 0;

public:
    void onDamage(int rawDamage, Monster&, scene::IngameBattle&);
    void onKilled(Monster&, scene::IngameBattle&);

protected:
    virtual void playHurtSfx(Monster&);

    virtual void reduceMobHp(int rawDamage, Monster&);
};

} // namespace ut::game::bt::mob
