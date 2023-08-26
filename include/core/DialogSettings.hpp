#pragma once

#include <bn_fixed_point.h>

namespace ut::asset
{
enum class FontKind;
enum class TextColorKind;
enum class SfxKind;
} // namespace ut::asset

namespace ut::core
{

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
    asset::FontKind font;
    asset::TextColorKind color;
    asset::SfxKind sfx;
    bn::fixed_point pos;
    int wrapWidth;
    bn::fixed lineHeight;
    int speed;
};

} // namespace ut::core
