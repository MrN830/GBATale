#include "core/Dialog.hpp"

#include <bn_display.h>

#include "asset/FontKind.hpp"
#include "asset/SfxKind.hpp"
#include "asset/TextColor.hpp"

namespace ut::core
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = -bn::fixed_point{bn::display::width() / 2, bn::display::height() / 2};

constexpr Dialog::Settings SETTINGS_PRESETS[(int)Dialog::Settings::Kind::TOTAL_COUNT] = {
    Dialog::Settings{
        asset::FontKind::MAIN,
        asset::TextColorKind::WHITE,
        asset::SfxKind::VOICE_INTRO,
        bn::fixed_point{39, 116} + TOP_LEFT_ORIGIN,
        176,
        14,
        2,
    },
    Dialog::Settings{
        asset::FontKind::MAIN,
        asset::TextColorKind::WHITE,
        asset::SfxKind::VOICE_INTRO,
        bn::fixed_point{61, 116} + TOP_LEFT_ORIGIN,
        176,
        14,
        2,
    },
    Dialog::Settings{
        asset::FontKind::MAIN,
        asset::TextColorKind::WHITE,
        asset::SfxKind::NONE,
        bn::fixed_point{48, 14} + TOP_LEFT_ORIGIN,
        176,
        14,
        1,
    },
    Dialog::Settings{
        asset::FontKind::MAIN,
        asset::TextColorKind::WHITE,
        asset::SfxKind::VOICE_DEFAULT,
        bn::fixed_point{10, 16} + TOP_LEFT_ORIGIN,
        225,
        14,
        1,
    },
    Dialog::Settings{
        asset::FontKind::MAIN,
        asset::TextColorKind::WHITE,
        asset::SfxKind::VOICE_DEFAULT,
        bn::fixed_point{10, 116} + TOP_LEFT_ORIGIN,
        225,
        14,
        1,
    },
};

} // namespace

auto Dialog::Settings::get(Kind kind) -> const Settings&
{
    BN_ASSERT(0 <= (int)kind && (int)kind < (int)Kind::TOTAL_COUNT);

    return SETTINGS_PRESETS[(int)kind];
}

} // namespace ut::core
