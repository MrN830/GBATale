#include "core/Dialog.hpp"

#include <bn_display.h>

#include "asset/FontKind.hpp"
#include "asset/SfxKind.hpp"
#include "asset/TextColor.hpp"

namespace ut::core
{

namespace
{

constexpr Dialog::Settings SETTINGS_PRESETS[(int)Dialog::Settings::Kind::TOTAL_COUNT] = {
    Dialog::Settings{
        asset::FontKind::MAIN,
        asset::TextColorKind::WHITE,
        asset::SfxKind::VOICE_INTRO,
        bn::fixed_point{35 - bn::display::width() / 2, 116 - bn::display::height() / 2},
        220,
        14,
        2,
    },
};

}

auto Dialog::Settings::get(Kind kind) -> const Settings&
{
    BN_ASSERT(0 <= (int)kind && (int)kind < (int)Kind::TOTAL_COUNT);

    return SETTINGS_PRESETS[(int)kind];
}

} // namespace ut::core
