#include "game/cpnt/WalkAnimCtrl.hpp"

#include <bn_sprite_item.h>

#include "asset/SpriteAnimInfo.hpp"
#include "asset/SpriteAnimKind.hpp"
#include "core/Directions.hpp"
#include "game/cmd/MoveCmd.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/cpnt/SpriteAnim.hpp"

namespace ut::game::cpnt
{

WalkAnimCtrl::WalkAnimCtrl(ent::Entity& entity, SpriteAnim& sprAnim)
    : Component(entity), _lastAnimDir(core::Directions::DOWN), _sprAnim(sprAnim),
      _walkAnimKind(asset::WalkAnimKind::NONE)
{
    sprAnim._walkAnimCtrl = this;
}

WalkAnimCtrl::~WalkAnimCtrl()
{
    // On entity delete, `WalkAnimCtrl` deleted first -> `SpriteAnim` deleted later.
    // So it should be safe  (btw, this dtor seems redundant?)
    _sprAnim._walkAnimCtrl = nullptr;
}

auto WalkAnimCtrl::getType() const -> bn::type_id_t
{
    return bn::type_id<WalkAnimCtrl>();
}

void WalkAnimCtrl::renderOnce()
{
    if (_sprAnim._action->current_index() == 1 && _curRenderCount > _sprAnim._action->wait_updates() + 1)
        _curRenderCount = 1;
    else
        ++_curRenderCount;
}

void WalkAnimCtrl::receiveMoveCmd(const cmd::MoveCmd& cmd)
{
    using AnimKind = asset::SpriteAnimKind;
    using IAnimInfo = asset::ISpriteAnimInfo;
    using Dirs = core::Directions;

    const bool prevAnimExist = (_sprAnim._curAnimKind != AnimKind::NONE);

    const auto nextMoveDirection = core::pos2Dirs(cmd.movePos);
    const auto prevAnimDirection = (prevAnimExist ? IAnimInfo::get(_sprAnim.getCurAnimKind()).directions : Dirs::NONE);

    // no need to change animation,
    // if next move direction contains current animation's move direction
    if (!!(nextMoveDirection & prevAnimDirection))
        return;

    BN_ASSERT(hasWalkAnim(), "no walk anim registered");
    const auto walk = asset::WalkAnimInfo::get(_walkAnimKind);

    // 1. BOTH actually moved + move requested direction
    if (!!(nextMoveDirection & Dirs::UP) && !!(cmd.directions & Dirs::UP))
        setCurAnimKind(walk.up, prevAnimExist);
    else if (!!(nextMoveDirection & Dirs::DOWN) && !!(cmd.directions & Dirs::DOWN))
        setCurAnimKind(walk.down, prevAnimExist);
    else if (!!(nextMoveDirection & Dirs::LEFT) && !!(cmd.directions & Dirs::LEFT))
        setCurAnimKind(walk.left, prevAnimExist);
    else if (!!(nextMoveDirection & Dirs::RIGHT) && !!(cmd.directions & Dirs::RIGHT))
        setCurAnimKind(walk.right, prevAnimExist);
    // 2. actually moved direction
    else if (!!(nextMoveDirection & Dirs::UP))
        setCurAnimKind(walk.up, prevAnimExist);
    else if (!!(nextMoveDirection & Dirs::DOWN))
        setCurAnimKind(walk.down, prevAnimExist);
    else if (!!(nextMoveDirection & Dirs::LEFT))
        setCurAnimKind(walk.left, prevAnimExist);
    else if (!!(nextMoveDirection & Dirs::RIGHT))
        setCurAnimKind(walk.right, prevAnimExist);
    // stop animate if stopped moving
    else
    {
        // 3. move requested but not moved (collided) direction
        if (!(cmd.directions & _lastAnimDir))
        {
            if (!!(cmd.directions & Dirs::UP))
                setCurAnimKind(walk.up, prevAnimExist);
            else if (!!(cmd.directions & Dirs::DOWN))
                setCurAnimKind(walk.down, prevAnimExist);
            else if (!!(cmd.directions & Dirs::LEFT))
                setCurAnimKind(walk.left, prevAnimExist);
            else if (!!(cmd.directions & Dirs::RIGHT))
                setCurAnimKind(walk.right, prevAnimExist);
        }

        // change sprite to stand still tile
        if (_sprAnim.getCurAnimKind() != AnimKind::NONE)
            setCurAnimKind(AnimKind::NONE, false);
    }
}

bool WalkAnimCtrl::hasWalkAnim() const
{
    return _walkAnimKind != asset::WalkAnimKind::NONE;
}

void WalkAnimCtrl::registerWalkAnimKind(asset::WalkAnimKind walkAnimKind)
{
    _walkAnimKind = walkAnimKind;
}

auto WalkAnimCtrl::getLastAnimDir() const -> core::Directions
{
    return _lastAnimDir;
}

void WalkAnimCtrl::setStandStillDir(core::Directions direction)
{
    using Dirs = core::Directions;

    BN_ASSERT(hasWalkAnim(), "no walk anim registered");
    const auto walk = asset::WalkAnimInfo::get(_walkAnimKind);

    if (!!(direction & Dirs::UP))
        setCurAnimKind(walk.up, false);
    else if (!!(direction & Dirs::DOWN))
        setCurAnimKind(walk.down, false);
    else if (!!(direction & Dirs::LEFT))
        setCurAnimKind(walk.left, false);
    else if (!!(direction & Dirs::RIGHT))
        setCurAnimKind(walk.right, false);

    setCurAnimKind(asset::SpriteAnimKind::NONE, false);
}

/**
 * @brief Set animation kind, and restart the animation if necessary.
 *
 * @param preserveRenderCount If a character turns while mid-walking,
 * it would look unnatural if you just restart the walking animation from beginning.
 * So, previous render count must be used to pre-render some cycles.
 */
void WalkAnimCtrl::setCurAnimKind(asset::SpriteAnimKind kind, bool preserveRenderCount)
{
    if (kind == asset::SpriteAnimKind::NONE)
    {
        if (hasWalkAnim() && _sprAnim.getCurAnimKind() != asset::SpriteAnimKind::NONE)
        {
            const auto& info = asset::ISpriteAnimInfo::get(_sprAnim.getCurAnimKind());
            const int standGfxIdx = info.getGfxIdxes()[1];
            _sprAnim._sprCpnt.getSprPtr().set_tiles(info.sprItem.tiles_item(), standGfxIdx);
        }

        // _sprAnim._curAnimKind = kind;
        _sprAnim._isManualRender = false;
        _sprAnim._action.reset();
        _curRenderCount = 0;
    }
    else
    {
        const auto& info = asset::ISpriteAnimInfo::get(kind);
        auto& spr = _sprAnim._sprCpnt.getSprPtr();

        if (preserveRenderCount)
        {
            BN_ASSERT(_sprAnim._action.has_value(), "no prev anim to preserve render count");
            BN_ASSERT(_sprAnim._action->update_forever(), "can't preserve render count for prev anim `ONCE`");
            BN_ASSERT(info.forever, "can't preserve render count for next anim `ONCE`");
            BN_ASSERT(_sprAnim._action->wait_updates() == info.waitUpdate,
                      "incompatible wait updates between prev=", _sprAnim._action->wait_updates(),
                      ", next=", info.waitUpdate);
        }

        _sprAnim._isManualRender = info.manualRender;

        _sprAnim._sprCpnt.setDiff(info.diff);
        spr.set_item(info.sprItem);
        spr.set_horizontal_flip(info.hFlip);
        spr.set_vertical_flip(info.vFlip);

        using SprAnimAction = decltype(_sprAnim._action)::value_type;

        // resolve a func ptr by name to an *overloaded* function, kinda ugly
        using CreateAnimFunc = SprAnimAction (*)(const bn::sprite_ptr&, int, const bn::sprite_tiles_item&,
                                                 const bn::span<const uint16_t>&);
        auto* createAnim = (info.forever ? static_cast<CreateAnimFunc>(SprAnimAction::forever)
                                         : static_cast<CreateAnimFunc>(SprAnimAction::once));

        _sprAnim._action = createAnim(spr, info.waitUpdate, info.sprItem.tiles_item(), info.getGfxIdxes());

        if (preserveRenderCount)
            for (int i = 0; i < _curRenderCount; ++i)
                _sprAnim._action->update();
    }

    if (hasWalkAnim())
    {
        const auto walk = asset::WalkAnimInfo::get(_walkAnimKind);

        // set last anim dir
        if (kind == walk.up)
            _lastAnimDir = core::Directions::UP;
        else if (kind == walk.down)
            _lastAnimDir = core::Directions::DOWN;
        else if (kind == walk.left)
            _lastAnimDir = core::Directions::LEFT;
        else if (kind == walk.right)
            _lastAnimDir = core::Directions::RIGHT;
    }

    _sprAnim._curAnimKind = kind;
}

} // namespace ut::game::cpnt
