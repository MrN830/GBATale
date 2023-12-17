#pragma once

#include <cstdint>

#include "asset/BgmKind.hpp"
#include "game/ItemInfo.hpp"

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

    uint32_t spared = 0;              // [23]
    uint32_t escaped = 0;             // [24]
    uint32_t dialogues_skipped = 0;   // [25]
    uint8_t murderlevel_override = 0; // [26]
    bool spared_specific : 1 = false; // [27]
    bool fast_text_skip : 1 = false;  // [28]

    bool tutorial_froggit_encountered : 1 = false; // [30]
    bool pushed_rock_1 : 1 = false;                // [31]
    bool pushed_rock_2 : 1 = false;                // [32]
    bool pushed_rock_3 : 1 = false;                // [33]
    uint8_t candy_taken = 0;                       // [34]
    bool pushed_rock_4 : 1 = false;                // [35]
    bool spared_napstablook : 1 = false;           // [36]

    enum class DogCallStatus : uint8_t
    {
        INIT = 0,
        DOG_KIDNAPPED_PHONE = 1,
        TORIEL_RECOVERED_PHONE = 2,
    };
    DogCallStatus dog_call_status = DogCallStatus::INIT; // [37]

    uint32_t greeted_toriel = 0;         // [40]
    uint32_t flirted_toriel = 0;         // [41]
    bool call_mom_toriel : 1 = false;    // [42]
    uint32_t ruins_switches_pressed = 0; // [43]
    uint32_t disobeyed_toriel = 0;       // [44]

    enum class StatusToriel : uint8_t
    {
        INIT = 0,
        LEAVE_RUINS = 1,
        FOUGHT = 3,
        KILLED = 4,
        SPARED = 5,
    };
    StatusToriel status_toriel = StatusToriel::INIT; // [45]

    enum class ChoiceFlavor : uint8_t
    {
        CINNAMON = 0,
        BUTTERSCOTCH = 1,
    };
    ChoiceFlavor choice_flavor = ChoiceFlavor::CINNAMON; // [46]
    int8_t status_creepy_tundra = 0;                     // [47]

    bool know_water_sausage : 1 = false; // [50]
    uint32_t wrong_switches_pressed = 0; // [51]

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

    enum class StatusSnowdrake : uint8_t
    {
        INIT = 0,
        LAUGHED = 1,
        KILLED = 2,
    };
    StatusSnowdrake status_snowdrake = StatusSnowdrake::INIT; // [57]

    enum class ChoiceHarderPuzzle : uint8_t
    {
        JUMBLE = 0,
        CROSSWORD = 1,
    };
    ChoiceHarderPuzzle choice_harder_puzzle = ChoiceHarderPuzzle::CROSSWORD; // [58]

    uint32_t spider_donations_total = 0;    // [59]
    uint32_t nicecream_donations_total = 0; // [60]

    enum class ChoiceAteLeftSpaghetti : uint8_t
    {
        INIT = 0,
        ATE = 1,
        REFUSE = 2,
    };
    ChoiceAteLeftSpaghetti choice_ate_left_spaghetti = ChoiceAteLeftSpaghetti::INIT; // [62]

    uint32_t xoxo_resets = 0;               // [63]
    bool toggled_snow_switch : 1 = false;   // [64]
    bool got_snowpoff_gold : 1 = false;     // [65]
    bool flirted_papyrus_fight : 1 = false; // [66]

    enum class StatusPapyrus : int8_t
    {
        INIT = 0,
        KILLED = 1,
        PAPYRUS_WINS_1 = -1,
        PAPYRUS_WINS_2 = -2,
        PAPYRUS_WINS_3 = -3,
    };
    StatusPapyrus status_papyrus = StatusPapyrus::INIT; // [67]

    bool fought_papyrus : 1 = false; // [68]

    uint32_t progress_tundra_battles = 0; // [70]

    enum class StatusInn : uint8_t
    {
        INIT = 0,
        STAY_INN = 1,
        STAY_INN_FREE = 2,
    };
    StatusInn status_inn = StatusInn::INIT; // [72]

    bool stayed_inn : 1 = false; // [73]

    ItemKind armor_papyrus_inquiry = ItemKind::NONE; // [75]
    bool choice_papyrus_inquiry : 1 = false;         // [76]
    ItemKind armor_undyne_saw = ItemKind::NONE;      // [77]
    bool strong_tough_glove : 1 = false;             // [78]
    bool nicecream_business : 1 = false;             // [79]
    uint32_t punchcards_bought = 0;                  // [80]

    enum class StatusShyren : uint8_t
    {
        INIT = 0,
        KILLED = 1,
        MAX_ENCOURAGE = 2,
    };
    StatusShyren status_shyren = StatusShyren::INIT; // [81]

    bool papyrus_sink_event_occurred : 1 = false; // [82]
    bool got_couch_gold : 1 = false;              // [83]

    bool have_umbrella : 1 = false;   // [85]
    bool music_statue_on : 1 = false; // [86]

    uint8_t dated_papyrus = 0; // [88]
    uint8_t dated_sans1 = 0;   // [89]

    enum class ChoiceMkidUmbrella : uint8_t
    {
        INIT = 0,
        NO_UMBRELLA_MET = 1,
        GET_UMBRELLA_AFTER_MET = 2,
        DROP_UMBRELLA_AFTER_GET = 3,
    };
    ChoiceMkidUmbrella choice_mkid_umbrella = ChoiceMkidUmbrella::INIT; // [90]

    bool interacted_garbage_savepoint : 1 = false; // [91]

    enum class DatedNapstablook : uint8_t
    {
        INIT = 0,
        TALK_IN_HOUSE = 1,
        FEEL_GARBAGE = 3,
        REFUSE_FEEL_GARBAGE = 9,
    };
    DatedNapstablook dated_napstablook = DatedNapstablook::INIT; // [93]

    asset::BgmKind current_napstablook_song = asset::BgmKind::NONE; // [94]

    bool aaron_woshua_event : 1 = false; // [95]
    uint32_t conversation_emblem = 0;    // [96]
    bool creepy_friend_seen : 1 = false; // [97]

    enum class SavedMkid : uint8_t
    {
        RAN_AWAY = 0,
        WATCH_UNDYNE_SAVES = 1,
        FRISK_SAVES = 2,
    };
    SavedMkid saved_mkid = SavedMkid::RAN_AWAY; // [98]

    uint32_t undyne_difficulty = 0; // [99]
    bool got_ribbon : 1 = false;    // [100]

    bool got_toyknife : 1 = false; // [102]

    enum class GotBscotchPie : uint8_t
    {
        INIT = 0,
        IN_ROOM = 1,
        GOT = 2,
    };
    GotBscotchPie got_bscotch_pie = GotBscotchPie::INIT; // [103]

    bool got_quiche : 1 = false;              // [104]
    bool got_tutu : 1 = false;                // [105]
    bool got_ballet_shoes : 1 = false;        // [106]
    bool got_artifact : 1 = false;            // [107]
    bool got_spacefood : 1 = false;           // [108]
    bool got_instant_noodles : 1 = false;     // [109]
    bool got_frying_pan : 1 = false;          // [110]
    bool got_apron : 1 = false;               // [111]
    bool got_glamburger_trashcan : 1 = false; // [112]
    bool got_gold_trashcan : 1 = false;       // [113]
    bool got_dagger : 1 = false;              // [114]
    bool got_locket : 1 = false;              // [115]

    bool spared_froggit : 1 = false;   // [130]
    bool spared_whimsun : 1 = false;   // [131]
    bool spared_moldsmal : 1 = false;  // [132]
    bool spared_loox : 1 = false;      // [133]
    bool spared_vegetoid : 1 = false;  // [134]
    bool spared_migosp : 1 = false;    // [135]
    bool spared_snowdrake : 1 = false; // [136]
    bool spared_icecap : 1 = false;    // [137]
    bool spared_gyftrot : 1 = false;   // [138]
    bool spared_doggo : 1 = false;     // [139]

    bool spared_lesserdog : 1 = false;    // [141]
    bool spared_greatdog : 1 = false;     // [142]
    bool spared_aaron : 1 = false;        // [143]
    bool spared_moldsmalx : 1 = false;    // [144]
    bool spared_woshua : 1 = false;       // [145]
    bool spared_temmie : 1 = false;       // [146]
    bool spared_maddummy : 1 = false;     // [147]
    bool spared_vulkin : 1 = false;       // [148]
    bool spared_tsunderplane : 1 = false; // [149]
    bool spared_pyrope : 1 = false;       // [150]
    bool spared_finalfroggit : 1 = false; // [151]
    bool spared_whimsalot : 1 = false;    // [152]
    bool spared_astigmatism : 1 = false;  // [153]
    bool spared_madjick : 1 = false;      // [154]
    bool spared_finalknight : 1 = false;  // [155]
    bool spared_endogeny : 1 = false;     // [156]

    uint32_t conversation_toriel_pacifist = 0;      // [191]
    uint32_t conversation_sans_pacifist = 0;        // [192]
    uint32_t conversation_undyne_pacifist = 0;      // [193]
    bool unlock_napsta_pacifist : 1 = false;        // [194]
    uint32_t conversation_papyrus_pacifist = 0;     // [195]
    uint32_t conversation_alphys_pacifist = 0;      // [196]
    uint32_t conversation_asgore_pacifist = 0;      // [197]
    uint32_t conversation_mettaton_pacifist = 0;    // [198]
    uint32_t conversation_napstablook_pacifist = 0; // [199]

    uint16_t kills = 0;        // [201]
    uint8_t kills_ruins = 0;   // [202]
    uint8_t kills_tundra = 0;  // [203]
    uint8_t kills_water = 0;   // [204]
    uint8_t kills_hotland = 0; // [205]

    bool genocide_ruins : 1 = false;   // [221]
    bool genocide_tundra : 1 = false;  // [222]
    bool genocide_water : 1 = false;   // [223]
    bool genocide_hotland : 1 = false; // [224]
    bool genocide_core : 1 = false;    // [225]

    bool killed_undyne_ex : 1 = false;  // [251]
    bool killed_glad_dummy : 1 = false; // [252]

    bool killed_muffet : 1 = false; // [397]

    bool killed_rg : 1 = false; // [402]

    bool killed_mettaton : 1 = false; // [425]

    uint32_t conversation_asriel2 = 0; // [511]
};

} // namespace ut::game
