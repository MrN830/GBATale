#include "game/InteractStateStack.hpp"

#include "game/GameContext.hpp"
#include "game/sys/TaskManager.hpp"

namespace ut::game
{

InteractStateStack::InteractStateStack(GameContext& ctx) : _gameContext(ctx)
{
}

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

    if (top() == InteractState::FREE)
        _gameContext.taskMngr.onSignal({task::TaskSignal::Kind::INTERACT_STATE_FREE});
}

auto InteractStateStack::top() const -> InteractState
{
    if (_stateStack.empty())
        return InteractState::FREE;

    return _stateStack.back();
}

} // namespace ut::game
