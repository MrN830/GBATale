#include "game/sys/TaskManager.hpp"

#include "game/task/TaskSignal.hpp"

#if UT_MEM_VIEW
#include "debug/MemView.hpp"
#endif

namespace ut::game::sys
{

TaskManager::TaskManager()
{
#if UT_MEM_VIEW
    debug::MemView::instance().setTaskMngr(this);
#endif
}

TaskManager::~TaskManager()
{
#if UT_MEM_VIEW
    debug::MemView::instance().setTaskMngr(nullptr);
#endif
}

void TaskManager::update()
{
    auto beforeIt = _tasks.before_begin();
    auto it = _tasks.begin();

    while (it != _tasks.end())
    {
        if (it->done())
        {
            it = _tasks.erase_after(beforeIt);
        }
        else
        {
            auto& signal = it->getTaskSignal();

            if (signal.kind == task::TaskSignal::Kind::TIME && --signal.number <= 0)
                it->resume();

            beforeIt = it;
            ++it;
        }
    }
}

void TaskManager::addTask(task::Task&& task)
{
    _tasks.push_front(std::move(task));
}

void TaskManager::onSignal(const task::TaskSignal& received)
{
    for (auto& task : _tasks)
    {
        const auto& signal = task.getTaskSignal();

        if (signal.kind != received.kind)
            continue;

        switch (received.kind)
        {
            using SigKind = task::TaskSignal::Kind;

        case SigKind::DIALOG_INDEX:
            if (signal.number == received.number)
                task.resume();
            break;

        case SigKind::DIALOG_CHOICE:
        case SigKind::DIALOG_END:
            task.resume();
            break;

        default:
            BN_ERROR("Invalid SigKind=", (int)received.kind, ", number=", (int)received.number);
        }
    }
}

} // namespace ut::game::sys
