#pragma once

#include <coroutine>

#include "game/task/TaskSignal.hpp"

namespace ut::game::task
{

class Task
{
public:
    struct promise_type;

    using CoHandle = std::coroutine_handle<promise_type>;

    struct promise_type
    {
        TaskSignal taskSignal;

        auto get_return_object() -> Task;
        auto initial_suspend() -> std::suspend_never;
        auto final_suspend() noexcept -> std::suspend_always;
        void unhandled_exception();
        void return_void();

        void* operator new(unsigned bytes) noexcept;
        void operator delete(void* ptr) noexcept;

        static auto get_return_object_on_allocation_failure() -> Task;
    };

public:
    Task(CoHandle);
    ~Task();

    Task(Task&& other) noexcept;
    Task& operator=(Task&& other) noexcept;

    Task(const Task&) = delete;
    Task& operator=(const Task&) = delete;

public:
    bool done() const;
    void resume();

    auto getTaskSignal() -> TaskSignal&;

private:
    CoHandle _coHandle;
};

} // namespace ut::game::task
