#include "scene/InputName.hpp"

#include <bn_display.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_string.h>

#include "asset/TextColor.hpp"
#include "core/Random.hpp"
#include "core/TextGens.hpp"
#include "game/GameState.hpp"
#include "util/String.hpp"

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
constexpr int GASTER_FRAMES = 4;

enum BtnIdx
{
    QUIT = 26 * 2,
    BACKSPACE,
    DONE,

    WORD_BUTTON_COUNT = 3
};

constexpr int getBtnIdx(char ch)
{
    if ('A' <= ch && ch <= 'Z')
        return ch - 'A';
    else if ('a' <= ch && ch <= 'z')
        return ch - 'a' + 26;

    BN_ERROR("Invalid ch=", ch);
    return -1;
}

constexpr char getCh(int btnIdx)
{
    if (getBtnIdx('A') <= btnIdx && btnIdx <= getBtnIdx('Z'))
        return 'A' + btnIdx;
    else if (getBtnIdx('a') <= btnIdx && btnIdx <= getBtnIdx('z'))
        return 'a' + (btnIdx - getBtnIdx('a'));

    BN_ERROR("Invalid btnIdx=", btnIdx);
    return '\0';
}

} // namespace

InputName::InputName(SceneStack& sceneStack, Context& context)
    : Scene(sceneStack, context), _inputText(context.gameState.getCharName().substr(0, 6))
{
    auto& textGen = context.textGens.get(asset::FontKind::MAIN);
    const auto prevAlign = textGen.alignment();

    updateInputTextSpr();
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

    select(getBtnIdx('A'));

    textGen.set_alignment(prevAlign);

    updateCharWobbles();
}

bool InputName::handleInput()
{
    if (isGaster())
        return true;

    handleArrowKeys();

    if (bn::keypad::b_pressed())
        eraseOneCharacter();

    if (bn::keypad::a_pressed())
        activate();

    return true;
}

bool InputName::update()
{
    if (isGaster())
    {
        if (_gasterCountdown-- == GASTER_FRAMES - 1)
        {
            _inputText.pop_back();
            updateInputTextSpr();
        }
        else if (_gasterCountdown <= 0)
        {
            _inputText.clear();
            updateCharNameInGameState();

            reqStackClear();
            reqStackPush(SceneId::INTRO_STORY);
        }
    }
    else
    {
        updateCharWobbles();
    }

    return true;
}

void InputName::select(int btnIdx)
{
    auto changeColorTo = [this](int idx, asset::TextColorKind colorKind) {
        const auto& color = asset::getTextColor(colorKind);

        if (0 <= idx && idx < BtnIdx::QUIT)
            _chars[idx].set_palette(color);
        else if (idx == BtnIdx::QUIT)
        {
            for (auto& text : _quit)
                text.set_palette(color);
        }
        else if (idx == BtnIdx::BACKSPACE)
        {
            for (auto& text : _backspace)
                text.set_palette(color);
        }
        else if (idx == BtnIdx::DONE)
        {
            for (auto& text : _done)
                text.set_palette(color);
        }
        else
            BN_ERROR("Invalid btnIdx=", idx);
    };

    changeColorTo(_selectedBtnIdx, asset::TextColorKind::WHITE);
    changeColorTo(btnIdx, asset::TextColorKind::YELLOW);

    _selectedBtnIdx = btnIdx;
}

void InputName::handleArrowKeys()
{
    if (bn::keypad::left_pressed() || bn::keypad::right_pressed())
    {
        const int diff = bn::keypad::left_pressed() ? -1 : +1;

        if (0 <= _selectedBtnIdx && _selectedBtnIdx < BtnIdx::QUIT)
        {
            const int btnIdx = bn::clamp(_selectedBtnIdx + diff, 0, BtnIdx::QUIT - 1);
            select(btnIdx);
        }
        else if (_selectedBtnIdx <= BtnIdx::DONE)
        {
            int btnIdx =
                ((_selectedBtnIdx + diff - BtnIdx::QUIT) + BtnIdx::WORD_BUTTON_COUNT) % BtnIdx::WORD_BUTTON_COUNT;
            btnIdx += BtnIdx::QUIT;
            select(btnIdx);
        }
        else
            BN_ERROR("Invalid btnIdx=", _selectedBtnIdx);
    }

    if (bn::keypad::up_pressed() || bn::keypad::down_pressed())
    {
        const int dir = bn::keypad::up_pressed() ? -1 : +1;

        if (_selectedBtnIdx == BtnIdx::QUIT)
        {
            if (dir == -1)
                select(getBtnIdx('s'));
            else
                select(getBtnIdx('A'));
        }
        else if (_selectedBtnIdx == BtnIdx::BACKSPACE)
        {
            if (dir == -1)
                select(getBtnIdx('v'));
            else
                select(getBtnIdx('D'));
        }
        else if (_selectedBtnIdx == BtnIdx::DONE)
        {
            if (dir == -1)
                select(getBtnIdx('r'));
            else
                select(getBtnIdx('I'));
        }
        // Up pressed on top row
        else if (dir == -1 && _selectedBtnIdx <= getBtnIdx('I'))
        {
            if (_selectedBtnIdx <= getBtnIdx('B'))
                select(BtnIdx::QUIT);
            else if (_selectedBtnIdx <= getBtnIdx('G'))
                select(BtnIdx::BACKSPACE);
            else
                select(BtnIdx::DONE);
        }
        // Down pressed on bottom character row
        else if (dir == +1 && getBtnIdx('r') <= _selectedBtnIdx && _selectedBtnIdx <= getBtnIdx('z'))
        {
            if (_selectedBtnIdx == getBtnIdx('r'))
                select(BtnIdx::DONE);
            else if (_selectedBtnIdx <= getBtnIdx('t'))
                select(BtnIdx::QUIT);
            else if (_selectedBtnIdx <= getBtnIdx('y'))
                select(BtnIdx::BACKSPACE);
            else
                select(BtnIdx::DONE);
        }
        // 'R' -> 'i'
        else if (dir == +1 && _selectedBtnIdx == getBtnIdx('R'))
        {
            select(getBtnIdx('i'));
        }
        // 'i' -> 'R'
        else if (dir == -1 && _selectedBtnIdx == getBtnIdx('i'))
        {
            select(getBtnIdx('R'));
        }
        // UPPER -> lower
        else if (dir == 1 && getBtnIdx('S') <= _selectedBtnIdx && _selectedBtnIdx <= getBtnIdx('Z'))
        {
            select(_selectedBtnIdx + (COLS - 1));
        }
        // lower -> UPPER
        else if (dir == -1 && getBtnIdx('a') <= _selectedBtnIdx && _selectedBtnIdx <= getBtnIdx('h'))
        {
            select(_selectedBtnIdx - (COLS - 1));
        }
        // normal up
        else if (dir == -1)
        {
            select(_selectedBtnIdx - COLS);
        }
        // normal down
        else // dir == +1
        {
            select(_selectedBtnIdx + COLS);
        }
    }
}

void InputName::eraseOneCharacter()
{
    if (!_inputText.empty())
    {
        _inputText.pop_back();
        updateInputTextSpr();
        updateCharNameInGameState();
    }
}

void InputName::activate()
{
    if (_selectedBtnIdx == BtnIdx::QUIT)
    {
        reqStackClear();
        reqStackPush(SceneId::NEW_GAME_TITLE);
    }
    else if (_selectedBtnIdx == BtnIdx::BACKSPACE)
    {
        eraseOneCharacter();
    }
    else if (_selectedBtnIdx == BtnIdx::DONE)
    {
        if (!_inputText.empty())
        {
            // TODO: Save name info to the Scene::Context

            reqStackClear();
            reqStackPush(SceneId::CONFIRM_NAME);
        }
    }
    // press alphabet key
    else
    {
        if (_inputText.full())
            _inputText.pop_back();

        _inputText.push_back(getCh(_selectedBtnIdx));
        updateInputTextSpr();
        updateCharNameInGameState();

        if (_inputText.size() == 6)
        {
            constexpr bn::string_view GASTER = "GASTER";

            auto str = util::toUpperAscii(_inputText);
            bn::string_view inputStr(str);

            if (inputStr == GASTER)
                _gasterCountdown = GASTER_FRAMES;
        }
    }
}

void InputName::updateInputTextSpr()
{
    _inputTextSpr.clear();

    auto& textGen = getContext().textGens.get(asset::FontKind::MAIN);
    textGen.generate(INPUT_TEXT_POS, _inputText, _inputTextSpr);
}

void InputName::updateCharNameInGameState()
{
    getContext().gameState.setCharName(_inputText);
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

bool InputName::isGaster() const
{
    return _gasterCountdown >= 0;
}

} // namespace ut::scene
