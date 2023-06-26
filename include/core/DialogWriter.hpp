#pragma once

#include <bn_display.h>
#include <bn_optional.h>
#include <bn_span.h>
#include <bn_sprite_ptr.h>
#include <bn_vector_fwd.h>

namespace ut::core
{

class TextGens;
struct Dialog;

struct SpecialToken
{
    enum class Kind
    {
        PAUSE,
        LINE_BREAK,
        STOP_WAIT_INPUT,
        STOP_WAIT_INPUT_CLOSE,
        NEXT_MSG,
        CLOSE,

        FACE_EMOTION,
        FACE_KIND,

        TEXT_CHOICE,

        DISABLE_TEXT_SOUND,
        ENABLE_TEXT_SOUND,
        PHONE_RING_SFX,

        TINY_TEXT,
        NORMAL_SIZE_TEXT,

        VOICE,
        WRITE_INFINITY_SIGN,
        WRITE_CONTROL_BUTTON,

        COLOR,

        TOTAL_COUNT
    };

    const Kind kind;
    const int number;
};

/**
 * @brief Render `sprite_text` line one character at a time.
 * You can also instantly render all text by calling `instantWrite()`
 */
class DialogWriter
{
public:
    DialogWriter(TextGens&, int bgPriority = 3);

    void reset();

    void start(bn::span<const Dialog> dialogs, bn::ivector<bn::sprite_ptr>& outputVec);
    bool isStarted() const;
    bool isWaitInput() const;

    void instantWrite();
    void keyInput();

    void update();

private:
    void resetStringProcessInfos();

    auto readSpecialToken(bn::string_view) -> bn::optional<SpecialToken>;
    void handleSpecialToken(const SpecialToken&);

    void nextDialog();

private:
    TextGens& _textGens;
    const int _bgPriority;

    bn::span<const Dialog> _dialogs;
    bn::ivector<bn::sprite_ptr>* _outputVec = nullptr;
    int _dialogIdx; // `-1` means not started
    int _nextCharIdx;
    int _curSpriteStartCharIdx;
    int _elapsedFrames;

    int _pauseCountdown;
    int _curLineWidth;
    int _sprLineWidth;
    bn::fixed _curLineY;
    bool _forceNewSprite;

    bool _isInstantWrite;
    bool _isWaitInput;
    bool _isCloseRequested;
};

} // namespace ut::core
