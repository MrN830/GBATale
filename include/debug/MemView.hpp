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
namespace ut::game::sys
{
class EntityManager;
class TaskManager;
} // namespace ut::game::sys

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
    void setEntMngr(game::sys::EntityManager*);
    void setTaskMngr(game::sys::TaskManager*);

private:
    void setVisible(bool);

    void redrawTexts();

private:
    static MemView* s_instance;

private:
    bn::sprite_text_generator& _textGen;
    game::sys::EntityManager* _entMngr = nullptr;
    game::sys::TaskManager* _taskMngr = nullptr;

    bn::vector<bn::sprite_ptr, 31> _texts;
    int _updateCountdown = 0;
    bool _isVisible = false;
};

} // namespace ut::debug
