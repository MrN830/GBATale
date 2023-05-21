#include "core/DialogWriter.hpp"

#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_vector.h>

#include "asset/SfxKind.hpp"
#include "asset/TextColor.hpp"
#include "core/Dialog.hpp"
#include "core/TextGens.hpp"

namespace ut::core
{

namespace
{
constexpr int MAX_SPRITE_WIDTH = 32;
} // namespace

DialogWriter::DialogWriter(TextGens& textGens) : _textGens(textGens)
{
}

void DialogWriter::start(bn::span<const Dialog> dialogs, bn::ivector<bn::sprite_ptr>& outputVec)
{
    if (dialogs.empty())
        return;

    _dialogs = dialogs;
    _outputVec = &outputVec;
    _dialogIdx = 0;
    resetStringProcessInfos();
}

bool DialogWriter::isStarted() const
{
    return 0 <= _dialogIdx && _dialogIdx < _dialogs.size();
}

void DialogWriter::update()
{
    if (!isStarted())
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
        const auto& settings = Dialog::Settings::get(dialog.settingsKind);

        auto& textGen = _textGens.get(settings.font);
        const auto prevAlign = textGen.alignment();
        textGen.set_left_alignment();

        // time to write a new character
        if (_elapsedFrames % settings.speed == 0 && _nextCharIdx < dialog.text.size())
        {
            auto specialToken = readSpecialToken(dialog.text);
            if (specialToken.has_value())
            {
                handleSpecialToken(*specialToken);
                _forceNewSprite = true;

                if (!isStarted())
                    return;

                continue;
            }

            const bn::string_view chStr = dialog.text.substr(_nextCharIdx, 1);
            const int chWidth = textGen.width(chStr);

            // get the starting position if this is very first char
            if (_curLineY <= -bn::display::height())
                _curLineY = settings.pos.y();

            // line wrapping
            if (_curLineWidth + chWidth > settings.wrapWidth)
            {
                _curLineWidth = 0;
                _sprLineWidth = 0;
                _forceNewSprite = true;
                _curLineY += settings.lineHeight;
            }

            const bn::fixed_point chPos(settings.pos.x() + _curLineWidth, _curLineY);
            const bn::fixed_point sprPos(settings.pos.x() + _sprLineWidth, _curLineY);

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
                }
            };

            if (_forceNewSprite)
            {
                generateNewSpriteText(textGen, chPos, chStr);
            }
            else
            {
                const int spriteCharCount = _nextCharIdx - _curSpriteStartCharIdx + 1;
                const auto spriteStr = dialog.text.substr(_curSpriteStartCharIdx, spriteCharCount);

                if (textGen.width(spriteStr) <= MAX_SPRITE_WIDTH)
                {
                    _outputVec->pop_back();
                    textGen.generate(sprPos, spriteStr, *_outputVec);
                }
                else
                {
                    generateNewSpriteText(textGen, chPos, chStr);
                }
            }

            _curLineWidth += chWidth;
            ++_nextCharIdx;
        }

        textGen.set_alignment(prevAlign);
        break;
    }

    ++_elapsedFrames;
}

void DialogWriter::resetStringProcessInfos()
{
    _nextCharIdx = 0;
    _curSpriteStartCharIdx = -1;
    _elapsedFrames = 0;
    _pauseCountdown = -1;
    _curLineWidth = 0;
    _sprLineWidth = 0;
    _curLineY = -bn::display::height();
    _forceNewSprite = true;
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
    else if (str.starts_with("&"))
    {
        _nextCharIdx += 1;
        return SpecialToken(SpecialToken::Kind::LINE_BREAK, UNUSED);
    }
    else if (str.starts_with("/%"))
    {
        _nextCharIdx += 2;
        return SpecialToken(SpecialToken::Kind::STOP_WAIT_INPUT_CLOSE, UNUSED);
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
    case SpecialToken::Kind::PAUSE:
        _pauseCountdown = specialToken.number * 10 - 1;
        break;

    case SpecialToken::Kind::LINE_BREAK: {
        const auto& dialog = _dialogs[_dialogIdx];
        const auto& settings = Dialog::Settings::get(dialog.settingsKind);

        _curLineY += settings.lineHeight;
        _curLineWidth = 0;
        _sprLineWidth = 0;
        _forceNewSprite = true;

        break;
    }

    case SpecialToken::Kind::NEXT_MSG: {
        _dialogIdx += 1;
        resetStringProcessInfos();
        _outputVec->clear();
        break;
    }

    case SpecialToken::Kind::FACE_EMOTION: {
        BN_LOG("SpecialToken::Kind::FACE_EMOTICON not implemented");
        break;
    }

    default:
        BN_ERROR("Not implemented SpecialToken::Kind = ", (int)specialToken.kind);
    }
}

} // namespace ut::core
