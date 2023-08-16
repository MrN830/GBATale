#include "game/cpnt/inter/HoleFall.hpp"

#include <bn_blending.h>
#include <bn_sound_item.h>

#include "asset/SfxKind.hpp"
#include "core/Directions.hpp"
#include "game/GameContext.hpp"
#include "game/GameState.hpp"
#include "game/RoomInfo.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/sys/CameraManager.hpp"
#include "game/sys/EntityManager.hpp"
#include "game/sys/RoomChanger.hpp"
#include "mtile/SpawnPoints.hpp"

#include "consts.hpp"

#include "gen/EntityId.hpp"

#include "bn_sprite_items_spr_hole.h"

namespace ut::game::cpnt::inter
{

namespace
{

constexpr int FRISK_FALL_WAIT_REDRAW = 10;
constexpr int WRONG_SWITCH_FALL_WAIT_REDRAW = 30;

constexpr int FRISK_TURN_WAIT_REDRAW = 4;

enum FriskGfxIdx
{
    DOWN = 1,
    UP = 4,
    LEFT = 6,
    RIGHT = 6 // horizontal flip required
};

} // namespace

HoleFall::HoleFall(ent::Entity& entity, bool isEnabled, InteractionTriggers triggers)
    : Interaction(entity, bn::type_id<HoleFall>(), isEnabled, triggers), _frisk(nullptr),
      _friskDir(core::Directions::NONE)
{
}

void HoleFall::awake(GameContext& ctx)
{
    _frisk = ctx.entMngr.findById(ent::gen::EntityId::frisk);
    BN_ASSERT(_frisk != nullptr);
}

auto HoleFall::onInteract(GameContext& ctx) -> task::Task
{
    Interaction::onInteract(ctx);

    if (ctx.interactStack.top() != InteractState::FREE)
        co_return;

    if (_holeSprs.full())
        _holeSprs.pop_front();

    const auto room = ctx.state.getRoom();
    auto& flags = ctx.state.getFlags();

    if (_holeSprs.size() >= 10 && room == RoomKind::ROOM_RUINS10)
    {
        _entity.setDestroyed(true);

        asset::getSfx(asset::SfxKind::HERO)->play();
    }
    else
    {
        asset::getSfx(asset::SfxKind::SWITCH_TRIGGER)->play();

        if (isWrongSwitchFall(ctx))
            flags.wrong_switches_pressed += 1;

        ctx.interactStack.push(InteractState::FALLING);

        _holeSprs.push_back(bn::sprite_items::spr_hole.create_sprite(_frisk->getPosition()));
        _holeSprs.back().set_z_order(32767);
        _holeSprs.back().set_bg_priority(consts::WORLD_BG_PRIORITY);
        _holeSprs.back().set_camera(ctx.camMngr.getCamera());

        const int waitRedraw = (isWrongSwitchFall(ctx) ? WRONG_SWITCH_FALL_WAIT_REDRAW : FRISK_FALL_WAIT_REDRAW);
        _redrawCountdown = waitRedraw + 1;
    }

    co_return;
}

void HoleFall::update(GameContext& ctx)
{
    Interaction::update(ctx);

    if (_friskMoveAction.has_value() && !_friskMoveAction->done())
    {
        _friskMoveAction->update();

        // end falling
        if (_friskMoveAction->done())
        {
            auto* friskSpr = _frisk->getComponent<cpnt::Sprite>();
            BN_ASSERT(friskSpr != nullptr);
            auto& friskSprPtr = friskSpr->getSprPtr();

            if (!isWrongSwitchFall(ctx))
            {
                bn::blending::set_transparency_alpha(1);
                friskSprPtr.set_blending_enabled(friskSpr->isBlendingEnabled());
            }

            friskSpr->setGfxIdx(FriskGfxIdx::DOWN);
            friskSprPtr.set_horizontal_flip(false);

            resetFriskFall();

            BN_ASSERT(ctx.interactStack.top() == InteractState::FALLING);
            ctx.interactStack.pop();
        }
    }
}

void HoleFall::render(GameContext& ctx)
{
    if (_redrawCountdown > 0)
    {
        if (--_redrawCountdown == 0)
        {
            _redrawCountdown = FRISK_TURN_WAIT_REDRAW;

            auto* friskSpr = _frisk->getComponent<cpnt::Sprite>();
            BN_ASSERT(friskSpr != nullptr);
            auto& friskSprPtr = friskSpr->getSprPtr();

            // start falling
            if (_friskDir == core::Directions::NONE)
            {
                asset::getSfx(asset::SfxKind::FALL)->play();

                if (!isWrongSwitchFall(ctx))
                {
                    friskSprPtr.set_blending_enabled(true);
                    bn::blending::set_transparency_alpha(0.5);
                }

                _friskDir = core::Directions::DOWN;
                friskSpr->setGfxIdx(FriskGfxIdx::DOWN);
                friskSprPtr.set_horizontal_flip(false);

                initFriskMoveAction();

                if (isWrongSwitchFall(ctx))
                {
                    BN_ASSERT(ctx.interactStack.top() == InteractState::FALLING);
                    ctx.interactStack.pop();

                    const mtile::Warp warp({}, RoomKind::ROOM_RUINS15E, mtile::WarpId::X, false);
                    ctx.roomChanger.reqChange(warp, ctx);
                }
            }
            // change Frisk sprite direction (already falling)
            else
            {
                using Dirs = core::Directions;

                const auto nextDir = (_friskDir == Dirs::DOWN    ? Dirs::RIGHT
                                      : _friskDir == Dirs::RIGHT ? Dirs::UP
                                      : _friskDir == Dirs::UP    ? Dirs::LEFT
                                                                 : Dirs::DOWN);

                const int nextGfxIdx = (nextDir == Dirs::UP     ? FriskGfxIdx::UP
                                        : nextDir == Dirs::DOWN ? FriskGfxIdx::DOWN
                                        : nextDir == Dirs::LEFT ? FriskGfxIdx::LEFT
                                                                : FriskGfxIdx::RIGHT);

                _friskDir = nextDir;
                friskSpr->setGfxIdx(nextGfxIdx);
                friskSprPtr.set_horizontal_flip(nextDir == Dirs::RIGHT);
            }
        }
    }
}

bool HoleFall::isWrongSwitchFall(GameContext& ctx) const
{
    const auto room = ctx.state.getRoom();

    return room == RoomKind::ROOM_RUINS15B || room == RoomKind::ROOM_RUINS15C || room == RoomKind::ROOM_RUINS15D;
}

void HoleFall::resetFriskFall()
{
    _redrawCountdown = -1;
    _friskDir = core::Directions::NONE;
    _friskMoveAction.reset();
}

void HoleFall::initFriskMoveAction()
{
    constexpr int FALL_M_TILE_COUNT = 20;
    constexpr int M_TILE_HEIGHT = 16;
    constexpr int FRISK_FALL_SPEED = 4;

    const auto finalPos = _frisk->getPosition() + bn::fixed_point(0, FALL_M_TILE_COUNT * M_TILE_HEIGHT);

    _friskMoveAction.emplace(*_frisk, FALL_M_TILE_COUNT * M_TILE_HEIGHT / FRISK_FALL_SPEED, finalPos);
}

} // namespace ut::game::cpnt::inter
