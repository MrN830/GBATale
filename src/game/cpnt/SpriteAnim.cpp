#include "game/cpnt/SpriteAnim.hpp"

#include <bn_sprite_item.h>

#include "asset/SpriteAnimInfo.hpp"
#include "asset/SpriteAnimKind.hpp"
#include "core/Directions.hpp"
#include "game/cmd/InputCmd.hpp"
#include "game/cpnt/Sprite.hpp"
#include "game/ent/Entity.hpp"

namespace ut::game::cpnt
{

SpriteAnim::SpriteAnim(ent::Entity& entity, cpnt::Sprite& sprCpnt)
    : Component(entity), _sprCpnt(sprCpnt), _up(asset::SpriteAnimKind::NONE), _down(asset::SpriteAnimKind::NONE),
      _left(asset::SpriteAnimKind::NONE), _right(asset::SpriteAnimKind::NONE),
      _curAnimKind(asset::SpriteAnimKind::NONE), _lastAnimDir(core::Directions::NONE)
{
}

auto SpriteAnim::getType() const -> bn::type_id_t
{
    return bn::type_id<SpriteAnim>();
}

void SpriteAnim::render(GameContext&)
{
    if (!_isManualRender)
        renderOnce();
}

void SpriteAnim::renderOnce()
{
    if (_action.has_value() && !_action->done())
    {
        _action->update();

        if (_action->current_index() == 1 && _curRenderCount > _action->wait_updates() + 1)
            _curRenderCount = 1;
        else
            ++_curRenderCount;
    }
}

bool SpriteAnim::isManualRender() const
{
    return _isManualRender;
}

void SpriteAnim::receiveInputCmd(const cmd::InputCmd& cmd)
{
    using AnimKind = asset::SpriteAnimKind;
    using IAnimInfo = asset::ISpriteAnimInfo;
    using Dirs = core::Directions;

    const bool prevAnimExist = (_curAnimKind != AnimKind::NONE);

    const auto nextMoveDirection = core::pos2Dirs(cmd.movePos);
    const auto prevAnimDirection = (prevAnimExist ? IAnimInfo::get(_curAnimKind).directions : Dirs::NONE);

    // no need to change animation,
    // if next move direction contains current animation's move direction
    if (!!(nextMoveDirection & prevAnimDirection))
        return;

    BN_ASSERT(hasDirectionAnims(), "no up/down/left/right anim registered");

    // 1. BOTH actually moved + move requested direction
    if (!!(nextMoveDirection & Dirs::UP) && !!(cmd.directions & Dirs::UP))
        setCurAnimKind(_up, prevAnimExist);
    else if (!!(nextMoveDirection & Dirs::DOWN) && !!(cmd.directions & Dirs::DOWN))
        setCurAnimKind(_down, prevAnimExist);
    else if (!!(nextMoveDirection & Dirs::LEFT) && !!(cmd.directions & Dirs::LEFT))
        setCurAnimKind(_left, prevAnimExist);
    else if (!!(nextMoveDirection & Dirs::RIGHT) && !!(cmd.directions & Dirs::RIGHT))
        setCurAnimKind(_right, prevAnimExist);
    // 2. actually moved direction
    else if (!!(nextMoveDirection & Dirs::UP))
        setCurAnimKind(_up, prevAnimExist);
    else if (!!(nextMoveDirection & Dirs::DOWN))
        setCurAnimKind(_down, prevAnimExist);
    else if (!!(nextMoveDirection & Dirs::LEFT))
        setCurAnimKind(_left, prevAnimExist);
    else if (!!(nextMoveDirection & Dirs::RIGHT))
        setCurAnimKind(_right, prevAnimExist);
    // stop animate if stopped moving
    else
    {
        // 3. move requested but not moved (collided) direction
        if (!(cmd.directions & _lastAnimDir))
        {
            if (!!(cmd.directions & Dirs::UP))
                setCurAnimKind(_up, prevAnimExist);
            else if (!!(cmd.directions & Dirs::DOWN))
                setCurAnimKind(_down, prevAnimExist);
            else if (!!(cmd.directions & Dirs::LEFT))
                setCurAnimKind(_left, prevAnimExist);
            else if (!!(cmd.directions & Dirs::RIGHT))
                setCurAnimKind(_right, prevAnimExist);
        }

        // change sprite to stand still tile
        if (_curAnimKind != AnimKind::NONE)
            setCurAnimKind(AnimKind::NONE, false);
    }
}

bool SpriteAnim::hasDirectionAnims() const
{
    return _up != asset::SpriteAnimKind::NONE;
}

void SpriteAnim::registerDirectionAnimKinds(asset::SpriteAnimKind up, asset::SpriteAnimKind down,
                                            asset::SpriteAnimKind left, asset::SpriteAnimKind right)
{
    _up = up;
    _down = down;
    _left = left;
    _right = right;
}

auto SpriteAnim::getCurAnimKind() const -> asset::SpriteAnimKind
{
    return _curAnimKind;
}

/**
 * @brief Set animation kind, and restart the animation if necessary.
 */
void SpriteAnim::setCurAnimKind(asset::SpriteAnimKind kind)
{
    setCurAnimKind(kind, false);
}

auto SpriteAnim::getLastAnimDir() const -> core::Directions
{
    return _lastAnimDir;
}

void SpriteAnim::setStandStillDir(core::Directions direction)
{
    BN_ASSERT(hasDirectionAnims(), "no up/down/left/right anim registered");

    using Dirs = core::Directions;

    if (!!(direction & Dirs::UP))
        setCurAnimKind(_up, false);
    else if (!!(direction & Dirs::DOWN))
        setCurAnimKind(_down, false);
    else if (!!(direction & Dirs::LEFT))
        setCurAnimKind(_left, false);
    else if (!!(direction & Dirs::RIGHT))
        setCurAnimKind(_right, false);

    setCurAnimKind(asset::SpriteAnimKind::NONE, false);
}

/**
 * @brief Set animation kind, and restart the animation if necessary.
 *
 * @param preserveRenderCount If a character turns while mid-walking,
 * it would look unnatural if you just restart the walking animation from beginning.
 * So, previous render count must be used to pre-render some cycles.
 */
void SpriteAnim::setCurAnimKind(asset::SpriteAnimKind kind, bool preserveRenderCount)
{
    if (kind == _curAnimKind)
        return;

    if (kind == asset::SpriteAnimKind::NONE)
    {
        if (hasDirectionAnims() && _curAnimKind != asset::SpriteAnimKind::NONE)
        {
            const auto& info = asset::ISpriteAnimInfo::get(_curAnimKind);
            const int standGfxIdx = info.getGfxIdxes()[1];
            _sprCpnt.getSprPtr().set_tiles(info.sprItem.tiles_item(), standGfxIdx);
        }

        // _curAnimKind = kind;
        _isManualRender = false;
        _action.reset();
        _curRenderCount = 0;
    }
    else
    {
        const auto& info = asset::ISpriteAnimInfo::get(kind);
        auto& spr = _sprCpnt.getSprPtr();

        if (preserveRenderCount)
        {
            BN_ASSERT(_action.has_value(), "no prev anim to preserve render count");
            BN_ASSERT(_action->update_forever(), "can't preserve render count for prev anim `ONCE`");
            BN_ASSERT(info.forever, "can't preserve render count for next anim `ONCE`");
            BN_ASSERT(_action->wait_updates() == info.waitUpdate,
                      "incompatible wait updates between prev=", _action->wait_updates(), ", next=", info.waitUpdate);
        }

        _isManualRender = info.manualRender;

        _sprCpnt.setDiff(info.diff);
        spr.set_item(info.sprItem);
        spr.set_horizontal_flip(info.hFlip);
        spr.set_vertical_flip(info.vFlip);

        using SprAnimAction = decltype(_action)::value_type;

        // resolve a func ptr by name to an *overloaded* function, kinda ugly
        using CreateAnimFunc = SprAnimAction (*)(const bn::sprite_ptr&, int, const bn::sprite_tiles_item&,
                                                 const bn::span<const uint16_t>&);
        auto* createAnim = (info.forever ? static_cast<CreateAnimFunc>(SprAnimAction::forever)
                                         : static_cast<CreateAnimFunc>(SprAnimAction::once));

        _action = createAnim(spr, info.waitUpdate, info.sprItem.tiles_item(), info.getGfxIdxes());

        if (preserveRenderCount)
            for (int i = 0; i < _curRenderCount; ++i)
                _action->update();
    }

    // set last anim dir
    if (kind == _up)
        _lastAnimDir = core::Directions::UP;
    else if (kind == _down)
        _lastAnimDir = core::Directions::DOWN;
    else if (kind == _left)
        _lastAnimDir = core::Directions::LEFT;
    else if (kind == _right)
        _lastAnimDir = core::Directions::RIGHT;

    _curAnimKind = kind;
}

} // namespace ut::game::cpnt
