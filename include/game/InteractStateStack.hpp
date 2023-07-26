#pragma once

#include <bn_vector.h>

namespace ut::game
{

enum class InteractState : int8_t
{
    FREE,
    INTERACT,
    CUTSCENE,
    ROOM_CHANGE,
    FALLING,
    MENU,
};

class InteractStateStack
{
public:
    void push(InteractState);
    void pop();

    auto top() const -> InteractState;

private:
    bn::vector<InteractState, 4> _stateStack;
};

} // namespace ut::game
