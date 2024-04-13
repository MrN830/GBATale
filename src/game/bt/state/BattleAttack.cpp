#include "game/bt/state/BattleAttack.hpp"

#include <bn_keypad.h>
#include <bn_math.h>
#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "core/Directions.hpp"
#include "core/Random.hpp"
#include "game/GameState.hpp"
#include "game/ItemInfo.hpp"
#include "game/bt/state/BattleStateType.hpp"
#include "scene/IngameBattle.hpp"

#include "game/bt/bt_consts.hpp"

#include "bn_sprite_items_bt_slice.h"

namespace ut::game::bt::state
{

namespace
{

constexpr int BAR_CREATE_DELAY = 6;
constexpr int MOB_DAMAGE_DELAY = 3;

constexpr bn::fixed ATK_SPEED = 11 * consts::ATK_BG_RATIO;
constexpr bn::fixed ATK_SPEED_RAND = 2 * consts::ATK_BG_RATIO;

constexpr bn::fixed BAR_Y = 50;
constexpr bn::fixed_point BAR_FAR_LEFT_POS = {-consts::ATK_BG_WIDTH / 2, BAR_Y};
constexpr bn::fixed_point BAR_FAR_RIGHT_POS = {consts::ATK_BG_WIDTH / 2, BAR_Y};

auto calcBarMoveDelta(core::Directions moveDir, game::ItemKind weaponKind, core::Random& rng) -> bn::fixed_point
{
    BN_ASSERT(moveDir == core::Directions::LEFT || moveDir == core::Directions::RIGHT,
              "Invalid moveDir=", (int)moveDir);

    bn::fixed x = ATK_SPEED + rng.get_fixed(ATK_SPEED_RAND);

    if (weaponKind == game::ItemKind::TOY_KNIFE)
        x *= bn::fixed(1.25f);

    if (moveDir == core::Directions::LEFT)
        x = -x;

    return {x, 0};
}

} // namespace

BattleAttack::BattleAttack(scene::IngameBattle& scene)
    : BattleState(scene), _state(State::WAIT_BAR_CREATION), _countdown(BAR_CREATE_DELAY),
      _barMoveDir((scene.getContext().rng.get_int(2)) ? core::Directions::LEFT : core::Directions::RIGHT),
      _bar((_barMoveDir == core::Directions::LEFT) ? BAR_FAR_RIGHT_POS : BAR_FAR_LEFT_POS),
      _barMoveAction(_bar,
                     calcBarMoveDelta(_barMoveDir, scene.getContext().gameState.getWeapon(), scene.getContext().rng)),
      _slice(bn::sprite_items::bt_slice.create_sprite(0, 0))
{
    scene.getAttackBg().setVisible(true);

    _bar.setVisible(false);
    _slice.set_visible(false);
}

auto BattleAttack::handleInput() -> BattleStateType
{
    if (_state != State::BAR_MOVING)
        return BattleStateType::NONE;

    if (bn::keypad::a_pressed())
    {
        _bar.startFlicker();

        // start slice
        asset::getSfx(asset::SfxKind::ATK_SLICE_SMALL)->play();

        const auto barCenterDist = bn::abs(_bar.getPosition().x());
        const auto stretch = (consts::ATK_BG_WIDTH - barCenterDist) / consts::ATK_BG_WIDTH;
        const auto sliceScale = (stretch * 2) - bn::fixed(0.5f);
        const auto sliceAnimSpeed = bn::max(bn::fixed(0.5f) - stretch / 4, bn::fixed(0.1f));
        const int sliceAnimWaitUpdate = (1 / sliceAnimSpeed).round_integer();

        _slice.set_scale(sliceScale);
        _slice.set_position(
            _scene.getMonsterManager().getMonsters()[_scene.getBtTempVars().submenuMobSelectIdx].getPosition());
        _slice.set_visible(true);
        _sliceAnim = bn::create_sprite_animate_action_once(_slice, sliceAnimWaitUpdate,
                                                           bn::sprite_items::bt_slice.tiles_item(), 0, 1, 2, 3, 4, 5);

        _state = State::SLICE_ONGOING;
    }

    return BattleStateType::NONE;
}

auto BattleAttack::update() -> BattleStateType
{
    _bar.render();

    switch (_state)
    {
    case State::WAIT_BAR_CREATION:
        return updateOnWaitBarCreation();
    case State::BAR_MOVING:
        return updateOnBarMoving();
    case State::SLICE_ONGOING:
        return updateOnSliceOngoing();
    case State::MOB_DAMAGE_ANIM_ONGOING:
        return updateOnMobDamageAnimOngoing();

    default:
        BN_ERROR("Invalid atkState=", (int)_state);
    }

    return BattleStateType::NONE;
}

auto BattleAttack::updateOnWaitBarCreation() -> BattleStateType
{
    if (--_countdown <= 0)
        _state = State::BAR_MOVING;

    return BattleStateType::NONE;
}

auto BattleAttack::updateOnBarMoving() -> BattleStateType
{
    if (!_bar.isVisible())
        _bar.setVisible(true);

    _barMoveAction.update();

    if (_barMoveDir == core::Directions::LEFT)
    {
        if (_bar.getPosition().x() < BAR_FAR_LEFT_POS.x())
        {
            // TODO: render `MISS`

            return BattleStateType::BATTLE_PREPARE_DODGE;
        }
    }
    else if (_barMoveDir == core::Directions::RIGHT)
    {
        if (_bar.getPosition().x() > BAR_FAR_RIGHT_POS.x())
        {
            // TODO: render `MISS`

            return BattleStateType::BATTLE_PREPARE_DODGE;
        }
    }
    else
        BN_ERROR("Invalid barMoveDir=", (int)_barMoveDir);

    return BattleStateType::NONE;
}

auto BattleAttack::updateOnSliceOngoing() -> BattleStateType
{
    if (_sliceAnim)
    {
        // 1-1. slice ongoing
        if (!_sliceAnim->done())
        {
            _sliceAnim->update();

            if (_sliceAnim->done())
                _countdown = _sliceAnim->wait_updates();
        }
        // 1-2. slice last frame
        else if (--_countdown <= 0)
        {
            _sliceAnim.reset();
            _slice.set_visible(false);

            _countdown = MOB_DAMAGE_DELAY;
        }
    }
    // 2. mob hurt delay
    else if (--_countdown <= 0)
    {
        // start mob damage react
        const int mobIdx = _scene.getBtTempVars().submenuMobSelectIdx;
        auto& mob = _scene.getMonsterManager().getMonsters()[mobIdx];

        mob.getReact().onDamage(calcDamage(), mob, _scene);

        _state = State::MOB_DAMAGE_ANIM_ONGOING;
    }

    return BattleStateType::NONE;
}

auto BattleAttack::updateOnMobDamageAnimOngoing() -> BattleStateType
{
    const int mobIdx = _scene.getBtTempVars().submenuMobSelectIdx;
    auto& mob = _scene.getMonsterManager().getMonsters()[mobIdx];

    // if mob damage anim is done
    if (mob.getAnim().getAnimKind() == mob::MonsterAnimKind::STOP)
    {
        // if mob is killed
        if (mob.getCurHp() <= 0)
        {
            mob.getReact().onKilled(mob, _scene);
        }
        else
        {
            mob.getAnim().start(mob::MonsterAnimKind::IDLE);
        }

        // TODO: check if all mobs are inactive
        // If so, end battle
        // If not, prepare dodge
        return BattleStateType::BATTLE_PREPARE_DODGE;
    }

    return BattleStateType::NONE;
}

int BattleAttack::calcDamage()
{
    // TODO: Implement actual damage calculation
    return 5;
}

} // namespace ut::game::bt::state
