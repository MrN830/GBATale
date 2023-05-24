#pragma once

#include <bn_fixed_point.h>
#include <bn_string_view.h>

namespace ut::asset
{
enum class FontKind;
enum class TextColorKind;
enum class SfxKind;
} // namespace ut::asset

namespace ut::core
{

struct Dialog final
{
public:
    struct Settings
    {
        enum class Kind
        {
            INTRO_STORY,
            INTRO_STORY_MT_EBOTT,
            CONFIRM_NAME,

            TOTAL_COUNT
        };

        static auto get(Kind) -> const Settings&;

        const asset::FontKind font;
        const asset::TextColorKind color;
        const asset::SfxKind sfx;
        const bn::fixed_point pos;
        const int wrapWidth;
        const bn::fixed lineHeight;
        const int speed;
    };

public:
    const Settings::Kind settingsKind;
    const bn::string_view text;
};

} // namespace ut::core
