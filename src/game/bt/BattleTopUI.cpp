#include "game/bt/BattleTopUI.hpp"

#include <bn_display.h>
#include <bn_format.h>
#include <bn_sprite_text_generator.h>

#include "core/TextGens.hpp"
#include "game/GameState.hpp"

#include "game/bt/bt_consts.hpp"

#include "bn_sprite_palette_items_pal_black_white.h"

namespace ut::game::bt
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = -bn::fixed_point{bn::display::width() / 2, bn::display::height() / 2};

constexpr auto NAME_POS = bn::fixed_point{6, 5} + TOP_LEFT_ORIGIN;
constexpr auto LV_POS = bn::fixed_point{51, 5} + TOP_LEFT_ORIGIN;
constexpr auto HP_NUM_POS = bn::fixed_point{235, 5} + TOP_LEFT_ORIGIN;

constexpr int HP_BAR_HEIGHT = 7;

inline constexpr int calcWidth(int hp)
{
    return (hp + 1) / 2;
}

} // namespace

BattleTopUI::BattleTopUI(const GameState& gameState, core::TextGens& textGens)
    : _gameState(gameState), _textGens(textGens),
      _hpBar({0, 0}, HP_BAR_HEIGHT, calcWidth(bn::max(gameState.getMaxHp(), gameState.getCurHp())),
             calcWidth(gameState.getCurHp()), bn::colors::yellow)
{
    auto& cryptGen = _textGens.get(asset::FontKind::CRYPT);
    const auto prevColor = cryptGen.palette_item();
    cryptGen.set_palette_item(bn::sprite_palette_items::pal_black_white);

    cryptGen.generate(NAME_POS, gameState.getCharName(), _nameSpr);

    cryptGen.set_palette_item(prevColor);

    for (auto& spr : _nameSpr)
    {
        spr.set_bg_priority(consts::BG_BOX_PRIORITY);
        spr.set_visible(_isVisible);
    }

    _hpBar.setVisible(_isVisible);

    redraw();
}

void BattleTopUI::redraw()
{
    _lvSpr.clear();
    _hpNumSpr.clear();
    _hpTextSpr.clear();

    const int curHp = _gameState.getCurHp();
    const int maxHp = _gameState.getMaxHp();

    const auto hpStr = bn::format<21>("{}/{}", curHp, maxHp);

    auto& cryptGen = _textGens.get(asset::FontKind::CRYPT);

    const auto prevColor = cryptGen.palette_item();

    cryptGen.set_palette_item(bn::sprite_palette_items::pal_black_white);

    cryptGen.generate(LV_POS, bn::format<13>("LV {}", _gameState.getLv()), _nameSpr);

    const auto prevAlign = cryptGen.alignment();
    cryptGen.set_right_alignment();

    cryptGen.generate(HP_NUM_POS, hpStr, _hpNumSpr);

    // `curHp` can exceed `maxHp`, if you visit the hotel.
    _hpBar.setWidth(calcWidth(bn::max(maxHp, curHp)), calcWidth(curHp));

    _hpBar.setTopLeftPos({
        HP_NUM_POS.x() - 9 - _textGens.get(asset::FontKind::TINY).width(hpStr) - calcWidth(bn::max(maxHp, curHp)),
        HP_NUM_POS.y() - 3,
    });

    cryptGen.generate(_hpBar.getTopLeftPos() + bn::fixed_point(-3, 3), "HP", _hpTextSpr);

    cryptGen.set_alignment(prevAlign);
    cryptGen.set_palette_item(prevColor);

    for (auto& spr : _lvSpr)
    {
        spr.set_bg_priority(consts::BG_BOX_PRIORITY);
        spr.set_visible(_isVisible);
    }
    for (auto& spr : _hpNumSpr)
    {
        spr.set_bg_priority(consts::BG_BOX_PRIORITY);
        spr.set_visible(_isVisible);
    }
    for (auto& spr : _hpTextSpr)
    {
        spr.set_bg_priority(consts::BG_BOX_PRIORITY);
        spr.set_visible(_isVisible);
    }
}

void BattleTopUI::setVisible(bool isVisible)
{
    _isVisible = isVisible;

    _hpBar.setVisible(isVisible);

    for (auto& spr : _nameSpr)
        spr.set_visible(isVisible);
    for (auto& spr : _lvSpr)
        spr.set_visible(isVisible);
    for (auto& spr : _hpNumSpr)
        spr.set_visible(isVisible);
    for (auto& spr : _hpTextSpr)
        spr.set_visible(isVisible);
}

} // namespace ut::game::bt
