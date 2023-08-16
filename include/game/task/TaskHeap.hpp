#pragma once

#include <bn_best_fit_allocator.h>

namespace ut::game::task
{

class TaskHeap
{
public:
    static auto instance() -> TaskHeap&;

private:
    TaskHeap();

public:
    auto getAllocator() -> bn::best_fit_allocator&;
    auto getAllocator() const -> const bn::best_fit_allocator&;

private:
    uint8_t _mem[4096];
    bn::best_fit_allocator _alloc;
};

} // namespace ut::game::task
