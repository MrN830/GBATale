#pragma once

#include <functional>

#include <bn_forward_list.h>

#include "game/task/Task.hpp"
#include "game/task/TaskAwaiters.hpp"

#include "config.hpp"

namespace ut::game
{
struct GameContext;
}

#if UT_MEM_VIEW
namespace ut::debug
{
class MemView;
}
#endif

namespace ut::game::sys
{

class TaskManager
{
#if UT_MEM_VIEW
    friend class ut::debug::MemView;
#endif

public:
    TaskManager();
    ~TaskManager();

    void update();

public:
    void addTask(task::Task&&);

    void onSignal(const task::TaskSignal&);

private:
    bn::forward_list<task::Task, 8> _tasks;
};

} // namespace ut::game::sys
