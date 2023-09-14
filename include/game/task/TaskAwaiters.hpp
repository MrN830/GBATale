#pragma once

#include "game/task/Task.hpp"

namespace ut::core
{
enum class DialogChoice;
}

namespace ut::game::task
{

class SignalAwaiter
{
public:
    SignalAwaiter(const task::TaskSignal& signal);

public:
    bool await_ready() const;
    void await_suspend(task::Task::CoHandle coHandle);
    void await_resume();

private:
    task::TaskSignal _signal;
};

class TimeAwaiter
{
public:
    TimeAwaiter(int ticks);

public:
    bool await_ready() const;
    void await_suspend(task::Task::CoHandle coHandle);
    void await_resume();

private:
    int _ticks;
};

class DialogChoiceAwaiter
{
public:
    bool await_ready() const;
    void await_suspend(task::Task::CoHandle coHandle);
    auto await_resume() -> core::DialogChoice;

private:
    task::Task::CoHandle _coHandle;
};

} // namespace ut::game::task
