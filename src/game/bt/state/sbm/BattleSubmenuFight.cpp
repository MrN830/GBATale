#include "game/bt/state/sbm/BattleSubmenuFight.hpp"

#include <bn_display.h>
#include <bn_format.h>
#include <bn_keypad.h>
#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "core/TextGens.hpp"
#include "game/bt/MonsterManager.hpp"
#include "game/bt/mob/MonsterInfo.hpp"
#include "game/bt/state/BattleMenu.hpp"
#include "game/bt/state/BattleStateType.hpp"
#include "game/bt/state/sbm/BattleSubmenuType.hpp"
#include "scene/IngameBattle.hpp"

#include "bn_sprite_items_spr_soul_heart.h"

namespace ut::game::bt::state::sbm
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = -bn::fixed_point{bn::display::width() / 2, bn::display::height() / 2};

constexpr bn::fixed_point NAME_POSS[3] = {
    bn::fixed_point{27, 116} + TOP_LEFT_ORIGIN,
    bn::fixed_point{27, 130} + TOP_LEFT_ORIGIN,
    bn::fixed_point{27, 144} + TOP_LEFT_ORIGIN,
};

constexpr bn::fixed_point NAME_HP_BAR_DIFF = {27, -3};
constexpr bn::fixed_point NAME_SOUL_HEART_DIFF = {-11, 0};

constexpr int HP_BAR_HEIGHT = 8;
constexpr int HP_BAR_TOTAL_WIDTH = 44;

inline constexpr int calcHealthWidth(int curHp, int maxHp)
{
    return curHp * HP_BAR_TOTAL_WIDTH / maxHp;
}

} // namespace

BattleSubmenuFight::BattleSubmenuFight(BattleMenu& priMenu)
    : BattleSubmenu(priMenu),
      _heartSpr(bn::sprite_items::spr_soul_heart.create_sprite(NAME_POSS[0] + NAME_SOUL_HEART_DIFF))
{
    const auto& mobs = priMenu.getScene().getMonsterManager().getMonsters();
    auto& textGen = priMenu.getScene().getContext().textGens.get(asset::FontKind::MAIN);

    bn::fixed hpBarX = NAME_POSS[0].x() + NAME_HP_BAR_DIFF.x();

    // render name texts
    for (int i = 0; i < mobs.size(); ++i)
    {
        const auto& mob = mobs[i];
        const auto& mobInfo = mob.getInfo();

        if (!mob.isActive())
            continue;

        const auto nameStr = bn::format<mob::MonsterInfo::MAX_NAME_LEN + 2>("* {}", mobInfo.getName());
        textGen.generate(NAME_POSS[i], nameStr, _nameSprs);

        const int nameWidth = textGen.width(nameStr);
        hpBarX = bn::max(hpBarX, NAME_POSS[i].x() + nameWidth + NAME_HP_BAR_DIFF.x());
    }

    // render hp bars
    for (int i = 0; i < mobs.size(); ++i)
    {
        const auto& mob = mobs[i];
        const auto& mobInfo = mob.getInfo();

        if (!mob.isActive())
            continue;

        _hpBars.emplace_back(consts::BG_ATK_PRIORITY, false, bn::fixed_point(hpBarX, NAME_POSS[i].y() + NAME_HP_BAR_DIFF.y()),
                             HP_BAR_HEIGHT, HP_BAR_TOTAL_WIDTH, calcHealthWidth(mob.getCurHp(), mobInfo.maxHp));

        _mobActiveFlags.set(i);
    }

    // set bg priority
    _heartSpr.set_bg_priority(consts::BG_ATK_PRIORITY);
    for (auto& spr : _nameSprs)
        spr.set_bg_priority(consts::BG_ATK_PRIORITY);

    BN_ASSERT(_mobActiveFlags.any(), "No monster is active");

    // adjust selected monster's idx if they are not active
    auto& selectedMobIdx = priMenu.getScene().getBtTempVars().submenuMobSelectIdx;
    for (int i = 0; !_mobActiveFlags.test(selectedMobIdx) && i < consts::MAX_MOBS; ++i)
        selectedMobIdx = (selectedMobIdx + 1) % consts::MAX_MOBS;

    updateHeartPos();
}

BattleSubmenuFight::~BattleSubmenuFight()
{
}

auto BattleSubmenuFight::handleInput() -> BattleSubmenuType
{
    if (bn::keypad::a_pressed())
    {
        asset::getSfx(asset::SfxKind::MENU_ACTIVATE)->play();

        _priMenu.setNextBattleState(BattleStateType::BATTLE_ATTACK);
    }
    else if (bn::keypad::b_pressed())
    {
        return BattleSubmenuType::MAIN;
    }
    else if (bn::keypad::up_pressed() || bn::keypad::down_pressed())
    {
        auto& selectedMobIdx = _priMenu.getScene().getBtTempVars().submenuMobSelectIdx;
        const int prevMobIdx = selectedMobIdx;
        const int dir = (bn::keypad::up_pressed()) ? -1 : +1;

        // move cursor to prev/next monster
        for (int i = 0; i < consts::MAX_MOBS; ++i)
        {
            selectedMobIdx = (selectedMobIdx + dir + consts::MAX_MOBS) % consts::MAX_MOBS;

            if (_mobActiveFlags.test(selectedMobIdx))
                break;
        }

        if (selectedMobIdx != prevMobIdx)
        {
            updateHeartPos();
            asset::getSfx(asset::SfxKind::MENU_CURSOR)->play();
        }
    }

    return BattleSubmenuType::NONE;
}

void BattleSubmenuFight::updateHeartPos()
{
    const auto selectedMobIdx = _priMenu.getScene().getBtTempVars().submenuMobSelectIdx;

    _heartSpr.set_position(NAME_POSS[selectedMobIdx] + NAME_SOUL_HEART_DIFF);
}

} // namespace ut::game::bt::state::sbm
