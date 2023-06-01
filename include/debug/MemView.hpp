#pragma once

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

namespace bn
{
class sprite_text_generator;
}
namespace ut::core
{
class TextGens;
}

namespace ut::debug
{

inline constexpr int IWRAM_BYTES = 32'768;
inline constexpr int EWRAM_BYTES = 262'144;

class MemView final
{
public:
    static constexpr int UPDATE_INTERVAL = 15;

public:
    MemView(core::TextGens&);

    void update();

private:
    void setVisible(bool);

    void redrawTexts();

private:
    bn::sprite_text_generator& _textGen;

    bn::vector<bn::sprite_ptr, 16> _texts;

    int _updateCountdown = 0;

    bool _isVisible = false;
};

} // namespace ut::debug
