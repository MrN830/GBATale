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
        ROOM_CHANGE,
        ENT_DESTROYED,

        TIME,

        CAM_SHAKE_END,

        SPR_ANIM_END,

        DIALOG_INDEX,
        DIALOG_CHOICE,
        DIALOG_END,

        NPC_WALK_END,
    };
};

} // namespace ut::game::task
