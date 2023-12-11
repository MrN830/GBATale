#include "core/DialogWriter.hpp"

#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_sound_item.h>
#include <bn_utf8_character.h>
#include <bn_vector.h>

#include "asset/PortraitInfo.hpp"
#include "asset/SfxKind.hpp"
#include "asset/TextColor.hpp"
#include "core/DialogChoice.hpp"
#include "core/TextGens.hpp"
#include "scene/SceneContext.hpp"

#include "consts.hpp"

#include "bn_sprite_items_spr_soul_heart.h"

namespace ut::core
{

namespace
{

constexpr int MAX_SPRITE_WIDTH = 32;

constexpr auto TEXT_CHOICE_HEART_DIFF = bn::fixed_point{-6, 0};

} // namespace

DialogWriter::DialogWriter(scene::SceneContext& context, int bgPriority)
    : _textGens(context.textGens), _bgPriority(bgPriority), _portrait(context.rng, bgPriority)
{
    reset();
}

void DialogWriter::reset()
{
    _dialogs = {};
    _outputVec = nullptr;
    _dialogIdx = -1; // `-1` means not started
    _nextCharIdx = -1;
    _curSpriteStartCharIdx = -1;
    _elapsedFrames = -1;

    _pauseCountdown = -1;
    _curLineWidth = -1;
    _sprLineWidth = -1;
    _curLineY = -bn::display::height();
    _forceNewSprite = false;
    _isInstantWrite = false;
    setWaitInput(false);
    _isCloseRequested = false;
}

void DialogWriter::start(bn::span<const bn::string_view> dialogs, const DialogSettings& settings,
                         bn::ivector<bn::sprite_ptr>& outputVec)
{
    if (dialogs.empty())
        return;

    _dialogs = dialogs;
    _settings = settings;
    _outputVec = &outputVec;
    _dialogIdx = 0;
    resetStringProcessInfos();

    _portrait.setDialogPos(settings.pos);
    _portrait.setInfo(asset::IPortraitInfo::get(settings.face, settings.emotion));
}

bool DialogWriter::isStarted() const
{
    return 0 <= _dialogIdx && _dialogIdx < _dialogs.size();
}

bool DialogWriter::isWaitInput() const
{
    return _isWaitInput;
}

bool DialogWriter::instantWrite()
{
    if (!isStarted())
        return false;

    const auto& dialog = _dialogs[_dialogIdx];

    // TODO: Optimize this super naive implementation
    const int prevCharIdx = _nextCharIdx;

    _isInstantWrite = true;
    while (isStarted() && !isWaitInput() && _nextCharIdx < dialog.size())
        update();
    _isInstantWrite = false;

    return prevCharIdx != _nextCharIdx;
}

auto DialogWriter::confirmKeyInput() -> DialogChoice
{
    if (!isWaitInput())
        return DialogChoice::NONE;

    setWaitInput(false);

    // text choice (e.g. yes/no)
    auto result = DialogChoice::NONE;
    if (_heartSpr.has_value())
    {
        _heartSpr.reset();

        result = (_isLeftSelected ? DialogChoice::LEFT : DialogChoice::RIGHT);
    }

    if (_isCloseRequested)
        reset();
    else
    {
        nextDialog();
        _portrait.onConfirmKeyInput();
    }

    return result;
}

void DialogWriter::update()
{
    _portrait.render();

    // text choice (e.g. yes/no)
    if (_heartSpr.has_value())
    {
        if (bn::keypad::left_pressed() || bn::keypad::right_pressed())
        {
            _isLeftSelected = !_isLeftSelected;
            _heartSpr->set_position(_isLeftSelected ? _leftChoicePos : _rightChoicePos);

            asset::getSfx(asset::SfxKind::MENU_CURSOR)->play();
        }

        return;
    }

    if (!isStarted() || isWaitInput())
        return;

    if (_pauseCountdown > 0)
    {
        --_pauseCountdown;
        return;
    }

    BN_ASSERT(0 <= _dialogIdx && _dialogIdx < _dialogs.size());

    while (true)
    {
        const auto& dialog = _dialogs[_dialogIdx];

        // fix width for choice option texts
        if (isCurDialogChoice() && !_isNextCharChoice && _isStartOfLine && _nextCharIdx < dialog.size())
        {
            const bn::utf8_character ch = dialog[_nextCharIdx];
            const bn::string_view chStr = dialog.substr(_nextCharIdx, ch.size());
            if (chStr == " ")
                _isNextCharChoice = true;

            _isStartOfLine = false;
        }

        auto& textGen = _textGens.get(_isNextCharChoice ? asset::FontKind::MAIN_FIXED_WIDTH : _settings.font);

        // time to write a new character
        if (_elapsedFrames % _settings.speed == 0 && _nextCharIdx < dialog.size())
        {
            auto specialToken = readSpecialToken(dialog);
            if (specialToken.has_value())
            {
                handleSpecialToken(*specialToken);
                _forceNewSprite = true;

                if (_nextCharIdx >= dialog.size())
                    setWaitInput(true);

                if (!isStarted() || isWaitInput())
                    return;

                continue;
            }

            const bn::utf8_character ch = dialog[_nextCharIdx];
            const bn::string_view chStr = dialog.substr(_nextCharIdx, ch.size());
            const int chWidth = textGen.width(chStr);

            // play sfx on writing non-whitespace character
            if (chStr != " " && !_isInstantWrite)
            {
                const auto sfx = asset::getSfx(_settings.sfx);
                if (sfx)
                    sfx->play();
            }

            // get the starting position if this is very first char
            if (_curLineY <= -bn::display::height())
                _curLineY = getMovedDialogPos().y();

            // line wrapping
            if (_curLineWidth + chWidth > _settings.wrapWidth - _portrait.getInfo().paddingX)
            {
                _curLineWidth = 0;
                _sprLineWidth = 0;
                _forceNewSprite = true;
                ++_curLineIdx;
                _curLineY += _settings.lineHeight;
            }

            const bn::fixed_point chPos(getMovedDialogPos().x() + _curLineWidth, _curLineY);
            const bn::fixed_point sprPos(getMovedDialogPos().x() + _sprLineWidth, _curLineY);

            // find text choice pos
            if (chStr != " " && _prevCharSpaceCnt >= 2)
            {
                _leftChoicePos = _rightChoicePos;
                _rightChoicePos = chPos + TEXT_CHOICE_HEART_DIFF;
            }

            auto generateNewSpriteText = [this](bn::sprite_text_generator& textGen_, const bn::fixed_point& chPos_,
                                                const bn::string_view& chStr_) {
                BN_ASSERT(!_outputVec->full(), "output vector is full, can't add more text sprites");

                const auto prevSize = _outputVec->size();
                textGen_.generate(chPos_, chStr_, *_outputVec);
                const bool isGenerated = (_outputVec->size() > prevSize);

                if (isGenerated)
                {
                    _forceNewSprite = false;
                    _curSpriteStartCharIdx = _nextCharIdx;
                    _sprLineWidth = _curLineWidth;
                    for (int i = prevSize; i < _outputVec->size(); ++i)
                        (*_outputVec)[i].set_bg_priority(_bgPriority);
                }
            };

            if (_forceNewSprite)
            {
                generateNewSpriteText(textGen, chPos, chStr);
            }
            else
            {
                const int spriteCharCount = _nextCharIdx - _curSpriteStartCharIdx + 1;
                const auto spriteStr = dialog.substr(_curSpriteStartCharIdx, spriteCharCount);

                if (textGen.width(spriteStr) <= MAX_SPRITE_WIDTH)
                {
                    _outputVec->pop_back();
                    textGen.generate(sprPos, spriteStr, *_outputVec);
                    _outputVec->back().set_bg_priority(_bgPriority);
                }
                else
                {
                    generateNewSpriteText(textGen, chPos, chStr);
                }
            }

            _curLineWidth += chWidth;
            _nextCharIdx += ch.size();
            _prevCharSpaceCnt = (chStr == " " ? _prevCharSpaceCnt + 1 : 0);
            if (_nextCharIdx >= dialog.size())
                setWaitInput(true);
        }

        break;
    }

    ++_elapsedFrames;
}

int DialogWriter::getCurDialogIdx() const
{
    return _dialogIdx;
}

void DialogWriter::setDialogPos(const bn::fixed_point& pos)
{
    BN_ASSERT(isStarted());
    BN_ASSERT(_outputVec != nullptr);

    const auto diff = pos - _settings.pos;
    for (auto& spr : *_outputVec)
        spr.set_position(spr.position() + diff);

    _settings.pos = pos;
}

auto DialogWriter::getMovedDialogPos() const -> bn::fixed_point
{
    return _settings.pos + bn::fixed_point{_portrait.getInfo().paddingX, 0};
}

void DialogWriter::resetStringProcessInfos()
{
    _nextCharIdx = 0;
    _curSpriteStartCharIdx = -1;
    _elapsedFrames = 0;
    _pauseCountdown = -1;
    _curLineWidth = 0;
    _sprLineWidth = 0;
    _curLineIdx = 0;
    _curLineY = -bn::display::height();
    _forceNewSprite = true;

    _isStartOfLine = true;
    _isNextCharChoice = false;

    _prevCharSpaceCnt = 0;
    _isLeftSelected = true;
    _heartSpr.reset();
}

auto DialogWriter::readSpecialToken(bn::string_view dialog) -> bn::optional<SpecialToken>
{
    constexpr int UNUSED = -1;

    auto str = dialog.substr(_nextCharIdx);

    if (str.starts_with("^"))
    {
        _nextCharIdx += 2;
        return SpecialToken(SpecialToken::Kind::PAUSE, str[1] - '0');
    }
    else if (str.starts_with("&") || str.starts_with("#"))
    {
        _nextCharIdx += 1;
        return SpecialToken(SpecialToken::Kind::LINE_BREAK, UNUSED);
    }
    else if (str.starts_with("/%"))
    {
        _nextCharIdx += 2;
        return SpecialToken(SpecialToken::Kind::STOP_WAIT_INPUT_CLOSE, UNUSED);
    }
    else if (str.starts_with("/*"))
    {
        _nextCharIdx += 2;
        return SpecialToken(SpecialToken::Kind::MENU_SELECTION, UNUSED);
    }
    else if (str.starts_with("/"))
    {
        _nextCharIdx += 1;
        return SpecialToken(SpecialToken::Kind::STOP_WAIT_INPUT, UNUSED);
    }
    else if (str.starts_with("%%"))
    {
        _nextCharIdx += 2;
        return SpecialToken(SpecialToken::Kind::CLOSE, UNUSED);
    }
    else if (str.starts_with("%"))
    {
        _nextCharIdx += 1;
        return SpecialToken(SpecialToken::Kind::NEXT_MSG, UNUSED);
    }
    else if (str.starts_with(R"(\[)"))
    {
        _nextCharIdx += 4;
        return SpecialToken(SpecialToken::Kind::VARIABLE, str[2]);
    }
    else if (str.starts_with(R"(\E)"))
    {
        _nextCharIdx += 3;
        return SpecialToken(SpecialToken::Kind::FACE_EMOTION, str[2] - '0');
    }
    else if (str.starts_with(R"(\F)"))
    {
        _nextCharIdx += 3;
        return SpecialToken(SpecialToken::Kind::FACE_KIND, str[2] - '0');
    }
    else if (str.starts_with(R"(\M)"))
    {
        _nextCharIdx += 3;
        return SpecialToken(SpecialToken::Kind::ANIM_INDEX, str[2] - '0');
    }
    else if (str.starts_with(R"(\C)"))
    {
        _nextCharIdx += 2;
        return SpecialToken(SpecialToken::Kind::TEXT_CHOICE, UNUSED);
    }
    else if (str.starts_with(R"(\S)"))
    {
        _nextCharIdx += 3;
        const char ch = str[2];
        return SpecialToken(ch == '-'   ? SpecialToken::Kind::DISABLE_TEXT_SOUND
                            : ch == '+' ? SpecialToken::Kind::ENABLE_TEXT_SOUND
                                        : SpecialToken::Kind::PHONE_RING_SFX,
                            UNUSED);
    }
    else if (str.starts_with(R"(\T)"))
    {
        _nextCharIdx += 3;
        const char ch = str[2];
        if (ch == '-')
            return SpecialToken(SpecialToken::Kind::TINY_TEXT, UNUSED);
        if (ch == '+')
            return SpecialToken(SpecialToken::Kind::NORMAL_SIZE_TEXT, UNUSED);

        return SpecialToken(SpecialToken::Kind::VOICE, ch == 'T'   ? (int)asset::SfxKind::VOICE_TORIEL
                                                       : ch == 't' ? (int)asset::SfxKind::VOICE_SANS_TORIEL
                                                       : ch == '0' ? (int)asset::SfxKind::VOICE_DEFAULT
                                                       : ch == 'S' ? (int)asset::SfxKind::VOICE_DEFAULT_NO_SOUND
                                                       : ch == 'F' ? (int)asset::SfxKind::VOICE_FLOWEY_EVIL
                                                       : ch == 's' ? (int)asset::SfxKind::VOICE_SANS
                                                       : ch == 'P' ? (int)asset::SfxKind::VOICE_PAPYRUS
                                                       : ch == 'M' ? (int)asset::SfxKind::VOICE_METTATON
                                                       : ch == 'U' ? (int)asset::SfxKind::VOICE_UNDYNE
                                                       : ch == 'A' ? (int)asset::SfxKind::VOICE_ALPHYS
                                                       : ch == 'a' ? (int)asset::SfxKind::VOICE_ASGORE
                                                                   : (int)asset::SfxKind::VOICE_ASRIEL);
    }
    else if (str.starts_with(R"(\z4)"))
    {
        _nextCharIdx += 3;
        return SpecialToken(SpecialToken::Kind::WRITE_INFINITY_SIGN, UNUSED);
    }
    else if (str.starts_with(R"(\*)"))
    {
        _nextCharIdx += 3;
        const char ch = str[2];
        // TODO: change to enum value
        return SpecialToken(SpecialToken::Kind::WRITE_CONTROL_BUTTON, ch == 'A'   ? (int)bn::keypad::key_type::LEFT
                                                                      : ch == 'D' ? (int)bn::keypad::key_type::RIGHT
                                                                      : ch == 'Z' ? (int)bn::keypad::key_type::A
                                                                      : ch == 'X' ? (int)bn::keypad::key_type::B
                                                                                  : (int)bn::keypad::key_type::START);
    }
    else if (str.starts_with(R"(\)"))
    {
        _nextCharIdx += 2;
        const char ch = str[1];
        return SpecialToken(SpecialToken::Kind::COLOR, ch == 'R'   ? (int)asset::TextColorKind::RED
                                                       : ch == 'G' ? (int)asset::TextColorKind::GREEN
                                                       : ch == 'W' ? (int)asset::TextColorKind::WHITE
                                                       : ch == 'Y' ? (int)asset::TextColorKind::YELLOW
                                                       : ch == 'X' ? (int)asset::TextColorKind::BLACK
                                                       : ch == 'B' ? (int)asset::TextColorKind::BLUE
                                                       : ch == 'O' ? (int)asset::TextColorKind::ORANGE
                                                       : ch == 'L' ? (int)asset::TextColorKind::AZURE
                                                       : ch == 'P' ? (int)asset::TextColorKind::MAGENTA
                                                                   : (int)asset::TextColorKind::PINK);
    }

    return bn::nullopt;
}

void DialogWriter::handleSpecialToken(const SpecialToken& specialToken)
{
    switch (specialToken.kind)
    {
    case SpecialToken::Kind::VARIABLE: {
        BN_LOG("SpecialToken::Kind::VARIABLE not implemented");
        break;
    }

    case SpecialToken::Kind::PAUSE:
        _pauseCountdown = specialToken.number * 10 - 1;
        break;

    case SpecialToken::Kind::LINE_BREAK: {
        ++_curLineIdx;
        _curLineY += _settings.lineHeight;
        _curLineWidth = 0;
        _sprLineWidth = 0;
        _forceNewSprite = true;

        _isStartOfLine = true;

        break;
    }

    case SpecialToken::Kind::STOP_WAIT_INPUT:
        setWaitInput(true);
        break;

    case SpecialToken::Kind::STOP_WAIT_INPUT_CLOSE:
        setWaitInput(true);
        _isCloseRequested = true;
        break;

    case SpecialToken::Kind::NEXT_MSG: {
        nextDialog();
        break;
    }

    case SpecialToken::Kind::CLOSE: {
        reset();
        break;
    }

    case SpecialToken::Kind::FACE_EMOTION: {
        const auto face = _portrait.getInfo().faceKind;
        const int emotion = specialToken.number;
        _portrait.setInfo(asset::IPortraitInfo::get(face, emotion));
        break;
    }

    case SpecialToken::Kind::FACE_KIND: {
        const auto face = (asset::PortraitFaceKind)specialToken.number;
        const int emotion = _portrait.getInfo().emotionKind;
        _portrait.setInfo(asset::IPortraitInfo::get(face, emotion));
        break;
    }

    case SpecialToken::Kind::TEXT_CHOICE: {
        _heartSpr = bn::sprite_items::spr_soul_heart.create_sprite(_leftChoicePos);
        _heartSpr->set_bg_priority(consts::INGAME_MENU_BG_PRIORITY);
        break;
    }

    case SpecialToken::Kind::ANIM_INDEX: {
        BN_LOG("SpecialToken::Kind::ANIM_INDEX not implemented");
        break;
    }

    case SpecialToken::Kind::VOICE: {
        _settings.sfx = (asset::SfxKind)specialToken.number;
        break;
    }

    case SpecialToken::Kind::COLOR: {
        if (_settings.font == asset::FontKind::MAIN)
        {
            const auto color = (asset::TextColorKind)specialToken.number;
            const auto& palette = asset::getTextColor(color);

            auto& textGen = _textGens.get(_settings.font);
            textGen.set_palette_item(palette);
        }
        else
            BN_ERROR("Font color change not implemented for font=", (int)_settings.font);

        break;
    }

    default:
        BN_ERROR("Not implemented SpecialToken::Kind = ", (int)specialToken.kind);
    }
}

void DialogWriter::nextDialog()
{
    _dialogIdx += 1;
    resetStringProcessInfos();
    _outputVec->clear();
}

bool DialogWriter::isCurDialogChoice() const
{
    BN_ASSERT(0 <= _dialogIdx && _dialogIdx < _dialogs.size());
    const auto& dialog = _dialogs[_dialogIdx];

    return dialog.ends_with(R"(\C)") || dialog.ends_with(R"(\C )");
}

void DialogWriter::setWaitInput(bool isWaitInput)
{
    if (_isWaitInput == isWaitInput)
        return;

    _isWaitInput = isWaitInput;

    if (isWaitInput)
        _portrait.onWaitInput();
}

} // namespace ut::core
