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
namespace ut::game::mngr
{
class EntityManager;
}

int main();

namespace ut::debug
{

inline constexpr int IWRAM_BYTES = 32'768;
inline constexpr int EWRAM_BYTES = 262'144;

class MemView final
{
    friend int ::main();

public:
    static auto instance() -> MemView&;

public:
    static constexpr int UPDATE_INTERVAL = 15;

private:
    MemView(core::TextGens&);

    void update();

public:
    void setEntMngr(game::mngr::EntityManager*);

private:
    void setVisible(bool);

    void redrawTexts();

private:
    static MemView* s_instance;

private:
    bn::sprite_text_generator& _textGen;
    game::mngr::EntityManager* _entMngr = nullptr;

    bn::vector<bn::sprite_ptr, 24> _texts;
    int _updateCountdown = 0;
    bool _isVisible = false;
};

} // namespace ut::debug
