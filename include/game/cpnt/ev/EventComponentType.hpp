#pragma once

#include <cstdint>

namespace ut::game::cpnt::ev
{

// Seperate type enum for jump table
enum class EventComponentType : uint8_t
{
    StartBgm,
    TimedDestroy,
    PlotSpike,
    SetPieImage,
    StalkerFlowey,
    TorielGoOutRuins1,
    CutsceneRuins3,
    CutsceneRuins5,
    CutsceneRuins6,
    CutsceneTorhouse1,
    CutsceneTorhouse3,
};

} // namespace ut::game::cpnt::ev
