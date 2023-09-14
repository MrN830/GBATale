#include "game/task/TaskHeap.hpp"

namespace ut::game::task
{

auto TaskHeap::instance() -> TaskHeap&
{
    static BN_DATA_EWRAM TaskHeap taskHeap;
    return taskHeap;
}

TaskHeap::TaskHeap() : _alloc(_mem, sizeof(_mem))
{
}

auto TaskHeap::getAllocator() -> bn::best_fit_allocator&
{
    return _alloc;
}

auto TaskHeap::getAllocator() const -> const bn::best_fit_allocator&
{
    return _alloc;
}

} // namespace ut::game::task
