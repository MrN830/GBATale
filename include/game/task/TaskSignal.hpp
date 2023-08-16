#pragma once

namespace ut::game::task
{

struct TaskSignal
{
    enum class Kind;

    Kind kind;
    int number = 0;

    enum class Kind
    {
        DIALOG_INDEX,
        DIALOG_CHOICE,
        DIALOG_END,

        TIME,
    };
};

} // namespace ut::game::task
