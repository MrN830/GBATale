#include "asset/PortraitInfo.hpp"

#include "asset/PortraitKind.hpp"

#include "bn_sprite_items_ui_portrait_flowey.h"
#include "bn_sprite_items_ui_portrait_toriel_head.h"
#include "bn_sprite_items_ui_portrait_toriel_torso.h"

namespace ut::asset
{

namespace
{

constexpr int TORIEL_PADDING_X = 52 - 3;
constexpr int TORIEL_WAIT_UPDATE = 4 - 1;

constexpr int FLOWEY_PADDING_X = 52 - 3;
constexpr int FLOWEY_WAIT_UPDATE = 5 - 1;

constexpr PortraitInfo<0, 0> null_portrait{
    PortraitFaceKind::NONE, 0, 0, 0, 0, nullptr, nullptr, nullptr, {}, {}, {}, {},
};

constexpr auto TORIEL_FACE_POS = bn::fixed_point{20, 11};
constexpr auto TORIEL_TORSO_POS = bn::fixed_point{21, 27};

constexpr auto FLOWEY_FACE_POS = bn::fixed_point{30, 24};

constexpr PortraitInfo<2, 3> toriel_happy{
    PortraitFaceKind::TORIEL,
    0,
    TORIEL_WAIT_UPDATE,
    4,
    TORIEL_PADDING_X,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_torso,
    TORIEL_FACE_POS,
    TORIEL_TORSO_POS,
    {4, 5},
    {6, 7, 4},
};

constexpr PortraitInfo<2, 3> toriel_side{
    PortraitFaceKind::TORIEL,
    1,
    TORIEL_WAIT_UPDATE,
    8,
    TORIEL_PADDING_X,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_torso,
    TORIEL_FACE_POS,
    TORIEL_TORSO_POS,
    {8, 9},
    {10, 11, 8},
};

constexpr PortraitInfo<2, 3> toriel_neutral{
    PortraitFaceKind::TORIEL,
    2,
    TORIEL_WAIT_UPDATE,
    0,
    TORIEL_PADDING_X,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_torso,
    TORIEL_FACE_POS,
    TORIEL_TORSO_POS,
    {0, 1},
    {2, 3, 0},
};

constexpr PortraitInfo<1, 0> toriel_what{
    PortraitFaceKind::TORIEL,
    3,
    TORIEL_WAIT_UPDATE,
    19,
    TORIEL_PADDING_X,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_torso,
    TORIEL_FACE_POS,
    TORIEL_TORSO_POS,
    {19},
    {},
};

constexpr PortraitInfo<1, 0> toriel_what_side{
    PortraitFaceKind::TORIEL,
    4,
    TORIEL_WAIT_UPDATE,
    20,
    TORIEL_PADDING_X,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_torso,
    TORIEL_FACE_POS,
    TORIEL_TORSO_POS,
    {20},
    {},
};

constexpr PortraitInfo<2, 0> toriel_revenge{
    PortraitFaceKind::TORIEL,
    5,
    TORIEL_WAIT_UPDATE,
    16,
    TORIEL_PADDING_X,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_torso,
    TORIEL_FACE_POS,
    TORIEL_TORSO_POS,
    {17, 18},
    {},
};

constexpr PortraitInfo<2, 2> toriel_cold{
    PortraitFaceKind::TORIEL,
    6,
    TORIEL_WAIT_UPDATE,
    12,
    TORIEL_PADDING_X,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_torso,
    TORIEL_FACE_POS,
    TORIEL_TORSO_POS,
    {12, 13},
    {13, 12},
};

constexpr PortraitInfo<2, 2> toriel_mad{
    PortraitFaceKind::TORIEL,
    7,
    TORIEL_WAIT_UPDATE,
    14,
    TORIEL_PADDING_X,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_torso,
    TORIEL_FACE_POS,
    TORIEL_TORSO_POS,
    {14, 15},
    {15, 14},
};

constexpr PortraitInfo<1, 0> toriel_embarrassed{
    PortraitFaceKind::TORIEL,
    8,
    TORIEL_WAIT_UPDATE,
    21,
    TORIEL_PADDING_X,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_torso,
    TORIEL_FACE_POS,
    TORIEL_TORSO_POS,
    {21},
    {},
};

constexpr PortraitInfo<1, 0> toriel_go_away_asgore{
    PortraitFaceKind::TORIEL,
    9,
    TORIEL_WAIT_UPDATE,
    22,
    TORIEL_PADDING_X,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_torso,
    TORIEL_FACE_POS,
    TORIEL_TORSO_POS,
    {22},
    {},
};

constexpr PortraitInfo<2, 0> flowey_nice{
    PortraitFaceKind::FLOWEY,
    0,
    FLOWEY_WAIT_UPDATE,
    0,
    FLOWEY_PADDING_X,
    &bn::sprite_items::ui_portrait_flowey,
    nullptr,
    nullptr,
    FLOWEY_FACE_POS,
    bn::fixed_point(),
    {0, 1},
    {},
};

constexpr PortraitInfo<2, 0> flowey_nice_side_um{
    PortraitFaceKind::FLOWEY,
    1,
    FLOWEY_WAIT_UPDATE,
    2,
    FLOWEY_PADDING_X,
    &bn::sprite_items::ui_portrait_flowey,
    nullptr,
    nullptr,
    FLOWEY_FACE_POS,
    bn::fixed_point(),
    {2, 3},
    {},
};

constexpr PortraitInfo<2, 0> flowey_sassy{
    PortraitFaceKind::FLOWEY,
    2,
    FLOWEY_WAIT_UPDATE,
    4,
    FLOWEY_PADDING_X,
    &bn::sprite_items::ui_portrait_flowey,
    nullptr,
    nullptr,
    FLOWEY_FACE_POS,
    bn::fixed_point(),
    {4, 5},
    {},
};

constexpr PortraitInfo<2, 0> flowey_pissed{
    PortraitFaceKind::FLOWEY,
    3,
    FLOWEY_WAIT_UPDATE,
    6,
    FLOWEY_PADDING_X,
    &bn::sprite_items::ui_portrait_flowey,
    nullptr,
    nullptr,
    FLOWEY_FACE_POS,
    bn::fixed_point(),
    {6, 7},
    {},
};

constexpr PortraitInfo<2, 0> flowey_evil{
    PortraitFaceKind::FLOWEY,
    4,
    FLOWEY_WAIT_UPDATE,
    8,
    FLOWEY_PADDING_X,
    &bn::sprite_items::ui_portrait_flowey,
    nullptr,
    nullptr,
    FLOWEY_FACE_POS,
    bn::fixed_point(),
    {8, 9},
    {},
};

constexpr PortraitInfo<2, 0> flowey_grin{
    PortraitFaceKind::FLOWEY,
    5,
    FLOWEY_WAIT_UPDATE,
    10,
    FLOWEY_PADDING_X,
    &bn::sprite_items::ui_portrait_flowey,
    nullptr,
    nullptr,
    FLOWEY_FACE_POS,
    bn::fixed_point(),
    {10, 11},
    {},
};

constexpr PortraitInfo<2, 0> flowey_toriel{
    PortraitFaceKind::FLOWEY,
    6,
    FLOWEY_WAIT_UPDATE,
    12,
    FLOWEY_PADDING_X,
    &bn::sprite_items::ui_portrait_flowey,
    nullptr,
    nullptr,
    FLOWEY_FACE_POS,
    bn::fixed_point(),
    {12, 13},
    {},
};

constexpr PortraitInfo<2, 0> flowey_toriel_2{
    PortraitFaceKind::FLOWEY,
    7,
    FLOWEY_WAIT_UPDATE,
    14,
    FLOWEY_PADDING_X,
    &bn::sprite_items::ui_portrait_flowey,
    nullptr,
    nullptr,
    FLOWEY_FACE_POS,
    bn::fixed_point(),
    {14, 15},
    {},
};

constexpr PortraitInfo<2, 0> flowey_plain{
    PortraitFaceKind::FLOWEY,
    8,
    FLOWEY_WAIT_UPDATE,
    16,
    FLOWEY_PADDING_X,
    &bn::sprite_items::ui_portrait_flowey,
    nullptr,
    nullptr,
    FLOWEY_FACE_POS,
    bn::fixed_point(),
    {16, 17},
    {},
};

} // namespace

auto IPortraitInfo::get(PortraitFaceKind face, uint8_t emotion) -> const IPortraitInfo&
{
    switch (face)
    {
        using Face = PortraitFaceKind;

    case Face::NONE:
        return null_portrait;

    case Face::TORIEL:
        if (emotion == 0)
            return toriel_happy;
        else if (emotion == 1)
            return toriel_side;
        else if (emotion == 2)
            return toriel_neutral;
        else if (emotion == 3)
            return toriel_what;
        else if (emotion == 4)
            return toriel_what_side;
        else if (emotion == 5)
            return toriel_revenge;
        else if (emotion == 6)
            return toriel_cold;
        else if (emotion == 7)
            return toriel_mad;
        else if (emotion == 8)
            return toriel_embarrassed;
        else if (emotion == 9)
            return toriel_go_away_asgore;

        return toriel_happy;

    case Face::FLOWEY:
        if (emotion == 0)
            return flowey_nice;
        else if (emotion == 1)
            return flowey_nice_side_um;
        else if (emotion == 2)
            return flowey_sassy;
        else if (emotion == 3)
            return flowey_pissed;
        else if (emotion == 4)
            return flowey_evil;
        else if (emotion == 5)
            return flowey_grin;
        else if (emotion == 6)
            return flowey_toriel;
        else if (emotion == 7)
            return flowey_toriel_2;
        else if (emotion == 8)
            return flowey_plain;

        return flowey_nice;

    default:
        BN_ERROR("Invalid face=", (int)face, ", emotion=", (int)emotion);
    }

    return null_portrait;
}

} // namespace ut::asset
