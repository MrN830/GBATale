#include "game/task/Task.hpp"

#include <bn_assert.h>

#include "game/task/TaskHeap.hpp"

namespace ut::game::task
{

auto Task::promise_type::get_return_object() -> Task
{
    return Task(CoHandle::from_promise(*this));
}

auto Task::promise_type::initial_suspend() -> std::suspend_never
{
    return {};
}

auto Task::promise_type::final_suspend() noexcept -> std::suspend_always
{
    return {};
}

void Task::promise_type::unhandled_exception()
{
    BN_ERROR("Task::promise_type has an unhandled exception");
}

void Task::promise_type::return_void()
{
}

void* Task::promise_type::operator new(unsigned bytes) noexcept
{
    auto& allocator = TaskHeap::instance().getAllocator();

    void* ptr = allocator.alloc(int(bytes));
    BN_ASSERT(ptr, "Task alloc failed: req=", bytes, ", free=", allocator.available_bytes());

    return ptr;
}

void Task::promise_type::operator delete(void* ptr) noexcept
{
    auto& allocator = TaskHeap::instance().getAllocator();

    allocator.free(ptr);
}

auto Task::promise_type::get_return_object_on_allocation_failure() -> Task
{
    BN_ERROR("Task alloc failed");
    return Task(nullptr);
}

Task::Task(CoHandle coHandle) : _coHandle(coHandle)
{
}

Task::~Task()
{
    if (_coHandle)
        _coHandle.destroy();
}

Task::Task(Task&& other) noexcept : _coHandle(std::move(other._coHandle))
{
    other._coHandle = nullptr;
}

Task& Task::operator=(Task&& other) noexcept
{
    _coHandle = std::move(other._coHandle);
    other._coHandle = nullptr;
    return *this;
}

bool Task::done() const
{
    return _coHandle.done();
}

void Task::resume()
{
    if (_coHandle && !_coHandle.done())
        _coHandle.resume();
}

auto Task::getTaskSignal() -> TaskSignal&
{
    return _coHandle.promise().taskSignal;
}

} // namespace ut::game::task
