#pragma once

#include <bn_vector.h>

#include "game/bt/mob/Monster.hpp"

#include "game/bt/bt_consts.hpp"

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
    bn::vector<mob::Monster, consts::MAX_MOBS> _monsters;

public:
    auto getMonsters() const -> decltype((_monsters));
};

} // namespace ut::game::bt
