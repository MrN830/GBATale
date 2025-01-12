#pragma once

#include <cstdint>

namespace ut::game::cpnt::inter
{

// Seperate type enum for jump table
enum class InteractionType : uint8_t
{
    SavePoint,
    AutoHideSpike,
    RuinsFloorSwitch,
    RuinsWallSwitch,
    RuinsColorSwitch,
    RuinsColorSwitchHelp,
    Readable,
    TalkFroggit,
    ItemPickup,
    HoleFall,
    HoleUp,
    MouseSqueak,
    CutsceneRuins2,
    TorielGoOutRuins2,
    TalkTorielRuins3,
    TalkTorielRuins4,
    TalkDummy,
    TalkTorielRuins6,
    TorielGoOutRuins5,
    TorielGoOutRuins6,
    RuinsTorielCall,
    CutsceneRuins19,
    Chairiel,
    AsrielLamp,
    CutsceneBasement1Block,
    CutsceneBasement1Proceed,
    CutsceneBasement2,
    CutsceneBasement3,
    TalkNpcArea1,
    FloweyTrigger1,
    FloweyTrigger2,
};

} // namespace ut::game::cpnt::inter
