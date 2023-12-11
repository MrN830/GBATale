#pragma once

#include <bn_vector.h>

namespace ut::game
{

struct GameContext;

enum class InteractState : int8_t
{
    FREE,
    INTERACT,
    DIALOG,
    SAVE_PROMPT,
    CUTSCENE,
    ROOM_CHANGE,
    FALLING,
    MENU,
};

class InteractStateStack
{
public:
    InteractStateStack(GameContext&);

public:
    void push(InteractState);
    void pop();

    auto top() const -> InteractState;

private:
    GameContext& _gameContext;

    bn::vector<InteractState, 4> _stateStack;
};

} // namespace ut::game
