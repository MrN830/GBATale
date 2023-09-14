#include "core/DialogPortrait.hpp"

#include <bn_sprite_item.h>

#include "asset/PortraitInfo.hpp"
#include "asset/PortraitKind.hpp"
#include "core/Random.hpp"

namespace ut::core
{

namespace
{
constexpr int TORSO_Z_ORDER = 1;
}

DialogPortrait::DialogPortrait(Random& rng, int bgPriority)
    : _rng(rng), _bgPriority(bgPriority), _info(&asset::IPortraitInfo::get(asset::PortraitFaceKind::NONE, 0))
{
}

void DialogPortrait::render()
{
    if (_faceAnim.has_value())
    {
        if (!_faceAnim->done())
            _faceAnim->update();
        else
            stopAnim();
    }
    else if (_isWaitInput && _info && _info->faceBlink)
    {
        if (_blinkCountdown > 0)
        {
            if (--_blinkCountdown == 0)
                startBlinkAnim();
        }
        else
        {
            _blinkCountdown = _rng.get_int(30, 91) + _info->waitUpdate;
        }
    }
}

void DialogPortrait::onWaitInput()
{
    _isWaitInput = true;

    if (_faceAnim)
    {
        stopAnim();

        BN_ASSERT(_info);
        if (_info->faceBlink)
            _blinkCountdown = _rng.get_int(20, 51) + _info->waitUpdate;
    }
}

void DialogPortrait::onConfirmKeyInput()
{
    _isWaitInput = false;

    if (_info && _info->faceKind != asset::PortraitFaceKind::NONE)
        startTalkAnim();
}

auto DialogPortrait::getInfo() const -> const asset::IPortraitInfo&
{
    BN_ASSERT(_info != nullptr);

    return *_info;
}

void DialogPortrait::setInfo(const asset::IPortraitInfo& info)
{
    if (&info == _info)
        return;

    _info = &info;

    if (info.faceKind == asset::PortraitFaceKind::NONE)
    {
        _faceAnim.reset();
        _face.reset();
        _torso.reset();
    }
    else
    {
        BN_ASSERT(info.faceTalk);
        _face = info.faceTalk->create_sprite(_dialogPos + info.faceDiff, info.faceIdleGfxIdx);
        _face->set_bg_priority(_bgPriority);

        if (info.torso)
        {
            _torso = info.torso->create_sprite(_dialogPos + info.torsoDiff);
            _torso->set_bg_priority(_bgPriority);
            _torso->set_z_order(TORSO_Z_ORDER);
        }

        startTalkAnim();
    }
}

void DialogPortrait::setDialogPos(const bn::fixed_point& pos)
{
    _dialogPos = pos;
}

void DialogPortrait::startTalkAnim()
{
    BN_ASSERT(_info);
    BN_ASSERT(_info->faceKind != asset::PortraitFaceKind::NONE);
    BN_ASSERT(_info->faceTalk);

    if (_info->getTalkGfxIdxes().size() > 1)
        _faceAnim = bn::sprite_animate_action<MAX_ANIM_FRAMES>::forever(
            *_face, _info->waitUpdate, _info->faceTalk->tiles_item(), _info->getTalkGfxIdxes());
}

void DialogPortrait::startBlinkAnim()
{
    BN_ASSERT(_info);
    BN_ASSERT(_info->faceKind != asset::PortraitFaceKind::NONE);
    BN_ASSERT(_info->faceBlink);

    if (_info->getBlinkGfxIdxes().size() > 1)
        _faceAnim = bn::sprite_animate_action<MAX_ANIM_FRAMES>::once(
            *_face, _info->waitUpdate, _info->faceBlink->tiles_item(), _info->getBlinkGfxIdxes());
}

void DialogPortrait::stopAnim()
{
    BN_ASSERT(_info);
    BN_ASSERT(_info->faceTalk);

    _faceAnim.reset();
    _face = _info->faceTalk->create_sprite(_dialogPos + _info->faceDiff, _info->faceIdleGfxIdx);
    _face->set_bg_priority(_bgPriority);
}

} // namespace ut::core
