#include "game/bt/state/BattlePrepareDodge.hpp"

#include "game/bt/state/BattleStateType.hpp"

#include "scene/IngameBattle.hpp"

#include "game/bt/bt_consts.hpp"

namespace ut::game::bt::state
{

namespace
{

constexpr bn::top_left_fixed_rect BG_BOX_DEST = {
    consts::BG_BOX_DODGE_DEFAULT_RECT.x(),
    consts::BG_BOX_INIT_RECT.y(),
    consts::BG_BOX_DODGE_DEFAULT_RECT.width(),
    consts::BG_BOX_INIT_RECT.height(),
};

} // namespace

BattlePrepareDodge::BattlePrepareDodge(scene::IngameBattle& scene) : BattleState(scene)
{
    scene.getMovingBgBox().moveTo(BG_BOX_DEST);

    if (scene.getAttackBg().isVisible())
        scene.getAttackBg().startFadeOut();
}

auto BattlePrepareDodge::handleInput() -> BattleStateType
{
    return BattleStateType::NONE;
}

auto BattlePrepareDodge::update() -> BattleStateType
{
    // TODO: add mob text done
    if (_scene.getMovingBgBox().isDoneMoving() && !_scene.getAttackBg().isVisible())
        return BattleStateType::BATTLE_DODGE;

    return BattleStateType::NONE;
}

} // namespace ut::game::bt::state
