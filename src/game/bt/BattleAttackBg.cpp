#include "game/bt/BattleAttackBg.hpp"

#include "game/bt/bt_consts.hpp"

#include "bn_affine_bg_items_bg_battle_attack.h"

namespace ut::game::bt
{

namespace
{

constexpr int ATK_BG_FADE_FRAMES = 15;

}

void BattleAttackBg::update()
{
    if (_shrink)
    {
        BN_ASSERT(_fade);

        if (!_shrink->done())
            _shrink->update();
        if (!_fade->done())
            _fade->update();

        if (_shrink->done() && _fade->done())
            setVisible(false);
    }
}

bool BattleAttackBg::isVisible() const
{
    return _bg.has_value();
}

void BattleAttackBg::setVisible(bool isVisible)
{
    if (isVisible)
    {
        _bg = bn::affine_bg_items::bg_battle_attack.create_bg(0, 0);
        _bg->set_wrapping_enabled(false);
        _bg->set_priority(consts::BG_ATK_PRIORITY);
    }
    else
    {
        if (_fade)
            bn::blending::set_transparency_alpha(1);

        _shrink.reset();
        _fade.reset();
        _bg.reset();
    }
}

void BattleAttackBg::startFadeOut()
{
    BN_ASSERT(isVisible(), "Tried to fade out invisible BattleAttackBg");
    _bg->set_blending_enabled(true);

    _shrink.emplace(*_bg, ATK_BG_FADE_FRAMES, 0.1f);
    _fade.emplace(ATK_BG_FADE_FRAMES, 0);
}

} // namespace ut::game::bt
