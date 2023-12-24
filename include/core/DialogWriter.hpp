#pragma once

#include <bn_display.h>
#include <bn_fixed_point.h>
#include <bn_optional.h>
#include <bn_sound_handle.h>
#include <bn_span.h>
#include <bn_sprite_ptr.h>
#include <bn_string_view.h>
#include <bn_vector_fwd.h>

#include "core/DialogPortrait.hpp"
#include "core/DialogSettings.hpp"

namespace ut::scene
{
struct SceneContext;
}

namespace ut::core
{

class TextGens;

enum class DialogChoice;

struct SpecialToken
{
    enum class Kind
    {
        VARIABLE,

        PAUSE,
        LINE_BREAK,
        STOP_WAIT_INPUT,
        STOP_WAIT_INPUT_CLOSE,
        NEXT_MSG,
        CLOSE,

        FACE_EMOTION,
        FACE_KIND,
        ANIM_INDEX,

        TEXT_CHOICE,
        MENU_SELECTION,

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
    DialogWriter(scene::SceneContext&, int bgPriority = 3);

    void reset();

    void start(bn::span<const bn::string_view> dialogs, const DialogSettings&, bn::ivector<bn::sprite_ptr>& outputVec);
    bool isStarted() const;
    bool isWaitInput() const;

    bool instantWrite();

    auto confirmKeyInput() -> DialogChoice;

    void update();

    int getCurDialogIdx() const;

public:
    void setDialogPos(const bn::fixed_point& pos);

private:
    // moved by portrait
    auto getMovedDialogPos() const -> bn::fixed_point;

private:
    void resetStringProcessInfos();

    auto readSpecialToken(bn::string_view) -> bn::optional<SpecialToken>;
    void handleSpecialToken(const SpecialToken&);

    void nextDialog();

private:
    bool isCurDialogChoice() const;

    void setWaitInput(bool isWaitInput);

private:
    void setTextColor(asset::TextColorKind);

private:
    TextGens& _textGens;
    const int _bgPriority;

    core::DialogPortrait _portrait;

    bn::optional<bn::sound_handle> _sfxHandle;

    bn::span<const bn::string_view> _dialogs;
    DialogSettings _settings;
    bn::ivector<bn::sprite_ptr>* _outputVec = nullptr;
    int _dialogIdx; // `-1` means not started
    int _nextCharIdx;
    int _curSpriteStartCharIdx;
    int _elapsedFrames;

    int _pauseCountdown;
    int _curLineWidth;
    int _sprLineWidth;
    int _curLineIdx;
    bn::fixed _curLineY;
    bool _forceNewSprite;

    bool _isInstantWrite;
    bool _isWaitInput;
    bool _isCloseRequested;

    // check whether to use fixed width (text choice)
    bool _isStartOfLine;
    bool _isNextCharChoice;

    // text choice (e.g. yes/no)
    bool _isLeftSelected;
    int _prevCharSpaceCnt;
    bn::fixed_point _leftChoicePos;
    bn::fixed_point _rightChoicePos;
    bn::optional<bn::sprite_ptr> _heartSpr;
};

} // namespace ut::core
