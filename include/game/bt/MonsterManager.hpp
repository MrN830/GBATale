#pragma once

#include <bn_vector.h>

#include "game/bt/mob/Monster.hpp"

namespace ut::game::bt
{

struct BattleContext;

class MonsterManager final
{
public:
    MonsterManager(const BattleContext&);

    void update();
    void render();

private:
    bn::vector<mob::Monster, 3> _monsters;
};

} // namespace ut::game::bt
