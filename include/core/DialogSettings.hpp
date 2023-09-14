#pragma once

#include <bn_fixed_point.h>
#include <bn_optional.h>

namespace ut::asset
{
enum class FontKind;
enum class TextColorKind;
enum class SfxKind;
enum class PortraitFaceKind : uint8_t;
} // namespace ut::asset

namespace ut::core
{

struct DialogSettingsOverride;

struct DialogSettings final
{
public:
    enum class PresetKind
    {
        INTRO_STORY,
        INTRO_STORY_MT_EBOTT,
        CONFIRM_NAME,

        WORLD_UPPER,
        WORLD_LOWER,

        TOTAL_COUNT
    };

public:
    static auto getPreset(PresetKind) -> const DialogSettings&;

public:
    void override(const DialogSettingsOverride&);

public:
    asset::FontKind font;
    asset::TextColorKind color;
    asset::SfxKind sfx;
    bn::fixed_point pos;
    int wrapWidth;
    bn::fixed lineHeight;
    uint8_t speed;
    asset::PortraitFaceKind face;
    uint8_t emotion;
};

struct DialogSettingsOverride final
{
public:
    enum class PresetKind
    {
        WORLD_TORIEL,

        TOTAL_COUNT
    };

public:
    static auto getPreset(PresetKind) -> const DialogSettingsOverride&;

public:
    void reset();

public:
    bn::optional<asset::FontKind> font;
    bn::optional<asset::TextColorKind> color;
    bn::optional<asset::SfxKind> sfx;
    bn::optional<bn::fixed_point> pos;
    bn::optional<int> wrapWidth;
    bn::optional<bn::fixed> lineHeight;
    bn::optional<uint8_t> speed;
    bn::optional<asset::PortraitFaceKind> face;
    bn::optional<uint8_t> emotion;
};

} // namespace ut::core
