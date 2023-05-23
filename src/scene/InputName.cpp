#include "scene/InputName.hpp"

#include <bn_display.h>
#include <bn_fixed_point.h>
#include <bn_string.h>

#include "asset/TextColor.hpp"
#include "core/Random.hpp"
#include "core/TextGens.hpp"

namespace ut::scene
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = bn::fixed_point{-bn::display::width() / 2, -bn::display::height() / 2};

constexpr auto INPUT_TEXT_POS = bn::fixed_point{96, 34} + TOP_LEFT_ORIGIN;

constexpr auto TIP_POS = bn::fixed_point{121, 14} + TOP_LEFT_ORIGIN;
constexpr auto A_POS = bn::fixed_point{45, 54} + TOP_LEFT_ORIGIN;
constexpr auto QUIT_POS = bn::fixed_point{30, 146} + TOP_LEFT_ORIGIN;
constexpr auto DONE_POS = bn::fixed_point{211, 146} + TOP_LEFT_ORIGIN;
constexpr auto BACKSPACE_POS = (QUIT_POS + DONE_POS) / 2;

constexpr bn::fixed_point CHAR_MARGIN = {18, 14};
constexpr bn::fixed CASE_MARGIN_Y = 2;

constexpr int ROWS = 6, COLS = 9;

} // namespace

InputName::InputName(SceneStack& sceneStack, Context& context) : Scene(sceneStack, context)
{
    auto& textGen = getContext().textGens.get(asset::FontKind::MAIN);
    const auto prevAlign = textGen.alignment();

    textGen.generate(QUIT_POS, "Quit", _quit);
    textGen.set_center_alignment();
    textGen.generate(TIP_POS, "Name the fallen human.", _tip);
    textGen.generate(BACKSPACE_POS, "Backspace", _backspace);
    textGen.set_right_alignment();
    textGen.generate(DONE_POS, "Done", _done);

    textGen.set_left_alignment();
    for (int y = 0; y < ROWS; ++y)
    {
        for (int x = 0; x < COLS; ++x)
        {
            const bool isLowerCase = (y >= ROWS / 2);
            const int chIdx = (isLowerCase ? (y - ROWS / 2) * COLS + x : y * COLS + x);
            if (chIdx >= 26)
                break;

            const auto chPos =
                A_POS + bn::fixed_point{x * CHAR_MARGIN.x(), y * CHAR_MARGIN.y() + (isLowerCase ? CASE_MARGIN_Y : 0)};
            const char ch = (isLowerCase ? 'a' + chIdx : 'A' + chIdx);

            textGen.generate(chPos, bn::to_string<1>(ch), _chars);
        }
    }

    _chars[0].set_palette(asset::getTextColor(asset::TextColorKind::YELLOW));

    textGen.set_alignment(prevAlign);

    updateCharWobbles();
}

bool InputName::handleInput()
{
    return true;
}

bool InputName::update()
{
    updateCharWobbles();

    return true;
}

void InputName::updateCharWobbles()
{
    auto& rng = getContext().rng;

    for (int isLowerCase = 0; isLowerCase <= 1; ++isLowerCase)
    {
        for (int chIdx = 0; chIdx < 26; ++chIdx)
        {
            // wobble chance: 2/3
            if (rng.get_fixed(1) <= bn::fixed(1.0 / 3))
                continue;

            auto& ch = _chars[isLowerCase * 26 + chIdx];

            const int xIdx = isLowerCase * 26 * 2 + chIdx * 2;
            const int yIdx = xIdx + 1;

            const bool prevWobbleX = _charWobbles[xIdx];
            const bool prevWobbleY = _charWobbles[yIdx];
            _charWobbles[xIdx] = rng.get_int(2);
            _charWobbles[yIdx] = rng.get_int(2);

            if (_charWobbles[xIdx] != prevWobbleX)
                ch.set_x(prevWobbleX ? ch.x() - 1 : ch.x() + 1);
            if (_charWobbles[yIdx] != prevWobbleY)
                ch.set_y(prevWobbleY ? ch.y() - 1 : ch.y() + 1);
        }
    }
}

} // namespace ut::scene
