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
        TIME,

        DIALOG_INDEX,
        DIALOG_CHOICE,
        DIALOG_END,

        NPC_WALK_END,
    };
};

} // namespace ut::game::task
