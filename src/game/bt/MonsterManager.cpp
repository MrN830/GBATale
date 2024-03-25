#include "game/bt/MonsterManager.hpp"

#include "game/bt/BattleContext.hpp"
#include "game/bt/BattleGroup.hpp"
#include "game/bt/mob/MonsterKind.hpp"

namespace ut::game::bt
{

MonsterManager::MonsterManager(const BattleContext& context)
{
    const auto& battleGroup = BattleGroup::get(context.battleGroupId);
    for (int i = 0; i < battleGroup.monsters.max_size(); ++i)
    {
        const auto mobKind = battleGroup.monsters[i];
        const auto mobPos = battleGroup.monsterPositions[i];

        if (mobKind == mob::MonsterKind::NONE)
            break;

        _monsters.emplace_back(mobKind, mobPos);
    }
}

void MonsterManager::update()
{
    for (auto& mob : _monsters)
        mob.update();
}

void MonsterManager::render()
{
    for (auto& mob : _monsters)
        mob.render();
}

auto MonsterManager::getMonsters() const -> decltype((_monsters))
{
    return _monsters;
}

} // namespace ut::game::bt
