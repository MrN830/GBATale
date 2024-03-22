#pragma once

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include "game/bt/HpBar.hpp"

namespace ut::core
{
class TextGens;
}
namespace ut::game
{
class GameState;
}

namespace ut::game::bt
{

class BattleTopUI
{
public:
    BattleTopUI(const GameState&, core::TextGens&);

    void redraw();

public:
    void setVisible(bool isVisible);

private:
    const GameState& _gameState;
    core::TextGens& _textGens;

    HpBar _hpBar;

    bn::vector<bn::sprite_ptr, 2> _nameSpr;
    bn::vector<bn::sprite_ptr, 2> _lvSpr;

    bn::vector<bn::sprite_ptr, 2> _hpNumSpr;
    bn::vector<bn::sprite_ptr, 1> _hpTextSpr;

    bool _isVisible = false;
};

} // namespace ut::game::bt
