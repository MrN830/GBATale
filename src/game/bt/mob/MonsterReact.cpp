#include "game/bt/mob/MonsterReact.hpp"

#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "game/bt/mob/Monster.hpp"
#include "game/bt/mob/MonsterInfo.hpp"
#include "scene/IngameBattle.hpp"

namespace ut::game::bt::mob
{

MonsterReact::~MonsterReact() = default;

void MonsterReact::onDamage(int rawDamage, Monster& mob, scene::IngameBattle& scene)
{
    BN_ASSERT(rawDamage >= 0, "rawDamage=", rawDamage, " shouldn't be negative");

    const int prevHp = mob.getCurHp();
    reduceMobHp(rawDamage, mob);
    const int curHp = mob.getCurHp();

    const auto& mobInfo = mob.getInfo();
    scene.getMobDamagePopup().start(mob.getPosition() + bn::fixed_point(0, mobInfo.hpBarDiffY), mobInfo.hpBarWidth,
                                    rawDamage, prevHp, curHp, mobInfo.maxHp);

    if (curHp < prevHp)
    {
        playHurtSfx(mob);
        mob.getAnim().start(MonsterAnimKind::HURT);
    }
}

void MonsterReact::onKilled(Monster& mob, scene::IngameBattle&)
{
    asset::getSfx(asset::SfxKind::VAPORIZED)->play();
    mob.getAnim().start(mob::MonsterAnimKind::KILLED);

    mob.setActive(false);
}

void MonsterReact::playHurtSfx(Monster& mob)
{
    using Sfx = asset::SfxKind;
    constexpr int CRY_DELAY = 11;

    asset::getSfx(Sfx::MOB_HURT_SMALL)->play();

    if (mob.getInfo().hurtCry != Sfx::NONE)
        asset::playSfxWithDelay(mob.getInfo().hurtCry, CRY_DELAY);
}

void MonsterReact::reduceMobHp(int rawDamage, Monster& mob)
{
    mob.setCurHp(bn::max(0, mob.getCurHp() - rawDamage));
}

} // namespace ut::game::bt::mob
