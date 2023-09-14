#include "game/task/TaskAwaiters.hpp"

#include <bn_assert.h>

#include "core/DialogChoice.hpp"

namespace ut::game::task
{

SignalAwaiter::SignalAwaiter(const task::TaskSignal& signal) : _signal(signal)
{
    BN_ASSERT(signal.kind != task::TaskSignal::Kind::TIME, "`TIME` signal must use `TimeAwaiter` instead");
    BN_ASSERT(signal.kind != task::TaskSignal::Kind::DIALOG_CHOICE,
              "`DIALOG_CHOICE` signal must use `DialogChoiceAwaiter` instead");
}

bool SignalAwaiter::await_ready() const
{
    return false;
}

void SignalAwaiter::await_suspend(task::Task::CoHandle coHandle)
{
    coHandle.promise().taskSignal = _signal;
}

void SignalAwaiter::await_resume()
{
}

TimeAwaiter::TimeAwaiter(int ticks) : _ticks(ticks)
{
}

bool TimeAwaiter::await_ready() const
{
    return _ticks <= 0;
}

void TimeAwaiter::await_suspend(task::Task::CoHandle coHandle)
{
    auto& signal = coHandle.promise().taskSignal;

    signal.kind = TaskSignal::Kind::TIME;
    signal.number = _ticks;
}

void TimeAwaiter::await_resume()
{
}

bool DialogChoiceAwaiter::await_ready() const
{
    return false;
}

void DialogChoiceAwaiter::await_suspend(task::Task::CoHandle coHandle)
{
    _coHandle = coHandle;

    auto& signal = coHandle.promise().taskSignal;
    signal.kind = TaskSignal::Kind::DIALOG_CHOICE;
    signal.number = (int)core::DialogChoice::NONE;
}

auto DialogChoiceAwaiter::await_resume() -> core::DialogChoice
{
    return (core::DialogChoice)_coHandle.promise().taskSignal.number;
}

} // namespace ut::game::task
