#pragma once

#include <cstdint>

namespace ut::game
{

/**
 * @brief Undertale's global.flag[513], See links below.
 * https://tomat.dev/undertale/flags
 * https://docs.google.com/document/d/1d5dt7A_jkTBhduVEGbWGtDbLEYMXCxSLUvqIqeXtYKw
 */
struct GameFlags
{
    bool undyne_trigger_override : 1 = false;   // [4]
    uint8_t fun = 0;                            // [5]
    bool hardmode : 1 = false;                  // [6]
    bool true_pacifist : 1 = false;             // [7]
    bool disable_random_encounters : 1 = false; // [8]

    bool spared_last : 1 = false;  // [10]
    bool escaped_last : 1 = false; // [11]
    bool killed_last : 1 = false;  // [12]
    bool bored_last : 1 = false;   // [13]

    enum class StatusDummy : uint8_t
    {
        RAN_AWAY = 0,
        KILLED = 1,
        TALKED = 2,
        BORED = 3,
    };
    StatusDummy status_dummy = StatusDummy::RAN_AWAY; // [14]

    bool in_battle : 1 = false;             // [15]
    bool type_heart_transition : 1 = false; // [16]
    bool menu_disabled : 1 = false;         // [17]
    bool view_rotated : 1 = false;          // [18]

    uint8_t animation_index = 0;     // [20]
    bool cooked_noodles : 1 = false; // [21]

    enum class NameColor : uint8_t
    {
        YELLOW = 0,
        WHITE = 1,
        PINK = 2,
    };
    NameColor name_color = NameColor::YELLOW; // [22]

    uint32_t spared = 0;                           // [23]
    uint32_t escaped = 0;                          // [24]
    uint32_t dialogues_skipped = 0;                // [25]
    uint8_t murderlevel_override = 0;              // [26]
    bool spared_specific : 1 = false;              // [27]
    bool fast_text_skip : 1 = false;               // [28]
    bool tutorial_froggit_encountered : 1 = false; // [29]
    bool pushed_rock_1 : 1 = false;                // [30]
    bool pushed_rock_2 : 1 = false;                // [31]
    bool pushed_rock_3 : 1 = false;                // [32]
    uint8_t candy_taken = 0;                       // [33]
    bool pushed_rock_4 : 1 = false;                // [34]
    bool spared_napstablook : 1 = false;           // [35]
    bool waited_toriel : 1 = false;                // [36]

    enum class DogCallStatus : uint8_t
    {
        INIT = 0,
        DOG_KIDNAPPED_PHONE = 1,
        TORIEL_RECOVERED_PHONE = 2,
    };
    DogCallStatus dog_call_status = DogCallStatus::INIT; // [37]

    uint16_t greeted_toriel = 0;         // [40]
    uint16_t flirted_toriel = 0;         // [41]
    bool call_mom_toriel : 1 = false;    // [42]
    uint16_t ruins_switches_pressed = 0; // [43]
    uint16_t disobeyed_toriel = 0;       // [44]

    enum class StatusToriel : uint8_t
    {
        INIT = 0,
        LEAVE_RUINS = 1,
        FOUGHT = 3,
        KILLED = 4,
        SPARED = 5,
    };
    StatusToriel status_toriel = StatusToriel::INIT; // [45]

    bool choice_flavor : 1 = false;  // [46]
    int8_t status_creepy_tundra = 0; // [47]

    bool know_water_sausage : 1 = false; // [50]
    uint16_t wrong_switches_pressed = 0; // [51]

    enum class StatusDoggo : uint8_t
    {
        INIT = 0,
        KILLED = 1,
        STICK_SPARE = 2,
    };
    StatusDoggo status_doggo = StatusDoggo::INIT; // [52]

    enum class StatusDogcouple : uint8_t
    {
        INIT = 0,
        KILLED = 1,
        DAMAGE_STICK = 2,
    };
    StatusDogcouple status_dogcouple = StatusDogcouple::INIT; // [53]

    enum class StatusGreaterdog : uint8_t
    {
        INIT = 0,
        KILLED = 1,
        STICK_SPARE = 2,
        REPEAT_IGNORE = 3,
    };
    StatusGreaterdog status_greaterdog = StatusGreaterdog::INIT; // [54]

    enum class StatusLesserdog : uint8_t
    {
        INIT = 0,
        KILLED = 1,
        PET_LONG_NECK = 2,
    };
    StatusLesserdog status_lesserdog = StatusLesserdog::INIT; // [55]

    enum class StatusSnowman : uint8_t
    {
        INIT = 0,
        GOT_PIECE = 1,
        TEARED_EXTRA_PIECE = 2,
        USED_IN_SIGHT = 4,
        TALKED_USED_IN_SIGHT = 5,
    };
    StatusSnowman status_snowman = StatusSnowman::INIT; // [56]

    bool got_bscotch_pie : 1 = false; // [103]

    uint8_t kills_ruins = 0; // [202]
};

} // namespace ut::game
