#include "game/InteractStateStack.hpp"

namespace ut::game
{

void InteractStateStack::push(InteractState kind)
{
    BN_ASSERT(!_stateStack.full(), "InteractStateStack is full, top=", (int)_stateStack.back(),
              ", push req=", (int)kind);

    _stateStack.push_back(kind);
}

void InteractStateStack::pop()
{
    BN_ASSERT(!_stateStack.empty(), "InteractStateStack is empty");

    _stateStack.pop_back();
}

auto InteractStateStack::top() const -> InteractState
{
    if (_stateStack.empty())
        return InteractState::FREE;

    return _stateStack.back();
}

} // namespace ut::game
