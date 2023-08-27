#include "core/DialogSettings.hpp"

#include <bn_display.h>

#include "asset/FontKind.hpp"
#include "asset/SfxKind.hpp"
#include "asset/TextColor.hpp"

namespace ut::core
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = -bn::fixed_point{bn::display::width() / 2, bn::display::height() / 2};

constexpr DialogSettings SETTINGS_PRESETS[(int)DialogSettings::PresetKind::TOTAL_COUNT] = {
    DialogSettings{
        asset::FontKind::MAIN,
        asset::TextColorKind::WHITE,
        asset::SfxKind::VOICE_INTRO,
        bn::fixed_point{39, 116} + TOP_LEFT_ORIGIN,
        176,
        14,
        2,
    },
    DialogSettings{
        asset::FontKind::MAIN,
        asset::TextColorKind::WHITE,
        asset::SfxKind::VOICE_INTRO,
        bn::fixed_point{61, 116} + TOP_LEFT_ORIGIN,
        176,
        14,
        2,
    },
    DialogSettings{
        asset::FontKind::MAIN,
        asset::TextColorKind::WHITE,
        asset::SfxKind::NONE,
        bn::fixed_point{48, 14} + TOP_LEFT_ORIGIN,
        176,
        14,
        1,
    },
    DialogSettings{
        asset::FontKind::MAIN,
        asset::TextColorKind::WHITE,
        asset::SfxKind::VOICE_DEFAULT,
        bn::fixed_point{10, 16} + TOP_LEFT_ORIGIN,
        225,
        14,
        1,
    },
    DialogSettings{
        asset::FontKind::MAIN,
        asset::TextColorKind::WHITE,
        asset::SfxKind::VOICE_DEFAULT,
        bn::fixed_point{10, 116} + TOP_LEFT_ORIGIN,
        225,
        14,
        1,
    },
};

constexpr DialogSettingsOverride SETTINGS_OVERRIDE_PRESETS[(int)DialogSettingsOverride::PresetKind::TOTAL_COUNT] = {
    DialogSettingsOverride{
        bn::nullopt,
        bn::nullopt,
        asset::SfxKind::VOICE_TORIEL,
        bn::nullopt,
        bn::nullopt,
        bn::nullopt,
        bn::nullopt,
    },
};

} // namespace

auto DialogSettings::getPreset(PresetKind kind) -> const DialogSettings&
{
    BN_ASSERT(0 <= (int)kind && (int)kind < (int)PresetKind::TOTAL_COUNT);

    return SETTINGS_PRESETS[(int)kind];
}

auto DialogSettingsOverride::getPreset(PresetKind kind) -> const DialogSettingsOverride&
{
    BN_ASSERT(0 <= (int)kind && (int)kind < (int)PresetKind::TOTAL_COUNT);

    return SETTINGS_OVERRIDE_PRESETS[(int)kind];
}

void DialogSettings::override(const DialogSettingsOverride& ovrd)
{
    font = (ovrd.font ? *ovrd.font : font);
    color = (ovrd.color ? *ovrd.color : color);
    sfx = (ovrd.sfx ? *ovrd.sfx : sfx);
    pos = (ovrd.pos ? *ovrd.pos : pos);
    wrapWidth = (ovrd.wrapWidth ? *ovrd.wrapWidth : wrapWidth);
    lineHeight = (ovrd.lineHeight ? *ovrd.lineHeight : lineHeight);
    speed = (ovrd.speed ? *ovrd.speed : speed);
}

void DialogSettingsOverride::reset()
{
    font.reset();
    color.reset();
    sfx.reset();
    pos.reset();
    wrapWidth.reset();
    lineHeight.reset();
    speed.reset();
}

} // namespace ut::core
