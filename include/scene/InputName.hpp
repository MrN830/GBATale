#pragma once

#include "scene/Scene.hpp"

#include <bn_bitset.h>
#include <bn_sprite_ptr.h>
#include <bn_string.h>
#include <bn_vector.h>

namespace ut::scene
{

class InputName final : public Scene
{
public:
    InputName(SceneStack&, Context&);

    bool handleInput() override;
    bool update() override;

private:
    void select(int btnIdx);
    void activate();

    void handleArrowKeys();
    void eraseOneCharacter();

    void updateInputTextSpr();
    void updateCharNameInGameState();

    void updateCharWobbles();

    bool isGaster() const;

private:
    bn::string<6> _inputText;
    bn::vector<bn::sprite_ptr, 2> _inputTextSpr;

    bn::vector<bn::sprite_ptr, 6> _tip;
    bn::vector<bn::sprite_ptr, 26 * 2> _chars;
    bn::vector<bn::sprite_ptr, 1> _quit;
    bn::vector<bn::sprite_ptr, 3> _backspace;
    bn::vector<bn::sprite_ptr, 1> _done;

    int _selectedBtnIdx = 0;

    bn::bitset<26 * 2 * 2> _charWobbles;
    int _gasterCountdown = -1;
};

} // namespace ut::scene
