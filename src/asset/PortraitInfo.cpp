#include "asset/PortraitInfo.hpp"

#include "asset/PortraitKind.hpp"

#include "bn_sprite_items_ui_portrait_toriel_head.h"
#include "bn_sprite_items_ui_portrait_toriel_torso.h"

namespace ut::asset
{

namespace
{

constexpr int DEFAULT_PADDING_X = 52 - 3;
constexpr int DEFAULT_WAIT_UPDATE = 4 - 1;

constexpr PortraitInfo<0, 0> null_portrait{
    PortraitFaceKind::NONE, 0, 0, 0, 0, nullptr, nullptr, nullptr, {}, {}, {}, {},
};

constexpr auto TORIEL_FACE_POS = bn::fixed_point{20, 11};
constexpr auto TORIEL_TORSO_POS = bn::fixed_point{21, 27};

constexpr PortraitInfo<2, 3> toriel_happy{
    PortraitFaceKind::TORIEL,
    0,
    DEFAULT_WAIT_UPDATE,
    4,
    DEFAULT_PADDING_X,
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
    DEFAULT_WAIT_UPDATE,
    8,
    DEFAULT_PADDING_X,
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
    DEFAULT_WAIT_UPDATE,
    0,
    DEFAULT_PADDING_X,
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
    DEFAULT_WAIT_UPDATE,
    19,
    DEFAULT_PADDING_X,
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
    DEFAULT_WAIT_UPDATE,
    20,
    DEFAULT_PADDING_X,
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
    DEFAULT_WAIT_UPDATE,
    16,
    DEFAULT_PADDING_X,
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
    DEFAULT_WAIT_UPDATE,
    12,
    DEFAULT_PADDING_X,
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
    DEFAULT_WAIT_UPDATE,
    14,
    DEFAULT_PADDING_X,
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
    DEFAULT_WAIT_UPDATE,
    21,
    DEFAULT_PADDING_X,
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
    DEFAULT_WAIT_UPDATE,
    22,
    DEFAULT_PADDING_X,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_head,
    &bn::sprite_items::ui_portrait_toriel_torso,
    TORIEL_FACE_POS,
    TORIEL_TORSO_POS,
    {22},
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

        goto INVALID_PORTRAIT;

    default:
    INVALID_PORTRAIT:
        BN_ERROR("Invalid face=", (int)face, ", emotion=", (int)emotion);
    }

    return null_portrait;
}

} // namespace ut::asset
