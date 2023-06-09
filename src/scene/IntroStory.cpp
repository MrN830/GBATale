#include "scene/IntroStory.hpp"

#include <bn_display.h>
#include <bn_dmg_music.h>
#include <bn_keypad.h>
#include <bn_music.h>
#include <bn_rect_window.h>
#include <bn_regular_bg_builder.h>
#include <bn_sound.h>

#include "core/Dialog.hpp"

#include "bn_regular_bg_items_bg_intro1.h"
#include "bn_regular_bg_items_bg_intro10.h"
#include "bn_regular_bg_items_bg_intro2.h"
#include "bn_regular_bg_items_bg_intro3.h"
#include "bn_regular_bg_items_bg_intro4.h"
#include "bn_regular_bg_items_bg_intro5.h"
#include "bn_regular_bg_items_bg_intro6.h"
#include "bn_regular_bg_items_bg_intro7.h"
#include "bn_regular_bg_items_bg_intro8.h"
#include "bn_regular_bg_items_bg_intro9.h"

#include "bn_dmg_music_items_once_upon_a_time.h"

namespace ut::scene
{

namespace
{

struct IntroBgFade
{
    const bn::regular_bg_item* bgItem;
    int fadeInStart;
    int fadeOutStart;
};

constexpr int FPS = 30;
constexpr int INTRO_BG_COUNT = 10;

constexpr int FADE_IN_FRAMES = 10;
constexpr int FADE_OUT_FRAMES = 9;
constexpr int LAST_FADE_IN_FRAMES = 7;
constexpr int LAST_FADE_OUT_FRAMES = 20;

constexpr int BG_MOVE_START_FRAME = 61 * FPS + 19;
constexpr int BG_MOVE_FRAMES = 229;

constexpr IntroBgFade INTRO_BGS[INTRO_BG_COUNT] = {
    {&bn::regular_bg_items::bg_intro1, -1, 6 * FPS + 15},
    {&bn::regular_bg_items::bg_intro2, 6 * FPS + 29, 12 * FPS + 3},
    {&bn::regular_bg_items::bg_intro3, 12 * FPS + 17, 17 * FPS + 27},
    {&bn::regular_bg_items::bg_intro4, 18 * FPS + 11, 24 * FPS + 0},
    {&bn::regular_bg_items::bg_intro5, 29 * FPS + 5, 34 * FPS + 0},
    {&bn::regular_bg_items::bg_intro6, 34 * FPS + 14, 40 * FPS + 15},
    {&bn::regular_bg_items::bg_intro7, 40 * FPS + 29, 45 * FPS + 18},
    {&bn::regular_bg_items::bg_intro8, 46 * FPS + 2, 51 * FPS + 3},
    {&bn::regular_bg_items::bg_intro9, 51 * FPS + 17, 56 * FPS + 21},
    {&bn::regular_bg_items::bg_intro10, 57 * FPS + 8, 74 * FPS + 27},
};

constexpr core::Dialog DIALOGS[] = {
    core::Dialog{
        core::Dialog::Settings::Kind::INTRO_STORY,
        R"(Long ago^1, two races&ruled over Earth^1:&HUMANS and MONSTERS^6.  ^1 %)",
    },
    core::Dialog{
        core::Dialog::Settings::Kind::INTRO_STORY,
        R"(One day^1, war broke&out between the two&races^6.  ^1 %)",
    },
    core::Dialog{
        core::Dialog::Settings::Kind::INTRO_STORY,
        R"(After a long battle^1,&the humans were&victorious^6.  ^1 %)",
    },
    core::Dialog{
        core::Dialog::Settings::Kind::INTRO_STORY,
        R"(They sealed the monsters&underground with a magic&spell^6.  ^1 %)",
    },
    core::Dialog{
        core::Dialog::Settings::Kind::INTRO_STORY,
        R"(Many years later^2. ^2. ^5. ^1 %)",
    },
    core::Dialog{
        core::Dialog::Settings::Kind::INTRO_STORY_MT_EBOTT,
        R"(      MT. EBOTT&         201X^9  %)",
    },
    core::Dialog{
        core::Dialog::Settings::Kind::INTRO_STORY,
        R"(Legends say that those&who climb the mountain&never return^5.^3  %)",
    },
};

constexpr int NEXT_SCENE_FRAMES = 75 * FPS + 27;
constexpr int SKIP_NEXT_SCENE_FRAMES = 30;

constexpr bn::fixed_point LAST_BG_START_POS = {8, -122};
constexpr bn::fixed_point LAST_BG_END_POS = {8, 91};

auto buildIntroBg(int idx0) -> bn::regular_bg_ptr
{
    BN_ASSERT(0 <= idx0 && idx0 < INTRO_BG_COUNT);

    bn::regular_bg_builder builder(*INTRO_BGS[idx0].bgItem);
    builder.set_blending_enabled(true);
    if (idx0 == INTRO_BG_COUNT - 1)
        builder.set_position(LAST_BG_START_POS);

    auto bg = builder.build();
    bn::rect_window::outside().set_show_bg(bg, false);

    return bg;
}

auto buildBgFadeIn(int frames) -> bn::blending_transparency_alpha_to_action
{
    return bn::blending_transparency_alpha_to_action(frames, 1);
}

auto buildBgFadeOut(int frames) -> bn::blending_transparency_alpha_to_action
{
    return bn::blending_transparency_alpha_to_action(frames, 0);
}

auto buildMusFadeOut(int frames) -> bn::dmg_music_volume_to_action
{
    return bn::dmg_music_volume_to_action(frames, 0);
}

} // namespace

IntroStory::IntroStory(SceneStack& sceneStack, Context& context)
    : Scene(sceneStack, context), _prevWindowRect(bn::rect_window::internal().boundaries()), _bg(buildIntroBg(0)),
      _bgFade(1, 1), _dialogWriter(getContext().textGens)
{
    bn::rect_window::internal().set_boundaries(6 - bn::display::height() / 2, 32 - bn::display::width() / 2,
                                               102 - bn::display::height() / 2, 208 - bn::display::width() / 2);

    if (bn::music::playing())
        bn::music::stop();
    if (bn::dmg_music::playing())
        bn::dmg_music::stop();

    const auto& introMusic = bn::dmg_music_items::once_upon_a_time;
    introMusic.play();

    _dialogWriter.start(DIALOGS, _texts);
}

IntroStory::~IntroStory()
{
    bn::blending::set_transparency_alpha(1);
    bn::rect_window::internal().set_boundaries(_prevWindowRect);

    bn::dmg_music::stop();
}

bool IntroStory::handleInput()
{
    // if skip button pressed
    if (!isSkipRequested() && bn::keypad::a_pressed())
    {
        _skipCountdown = SKIP_NEXT_SCENE_FRAMES;
        _isFadeOut = true;
        _bgFade = buildBgFadeOut(LAST_FADE_OUT_FRAMES);
        _musFade = buildMusFadeOut(LAST_FADE_OUT_FRAMES);
        _texts.clear();

        bn::sound::stop_all();
    }

    return true;
}

bool IntroStory::update()
{
    ++_elapsedFrames;

    if (isSkipRequested())
    {
        if (!_bgFade.done())
            _bgFade.update();

        if (--_skipCountdown == 0)
        {
            reqStackClear();
            reqStackPush(SceneId::INTRO_LOGO);
        }
    }
    else
    {
        updateBgFade();
        _dialogWriter.update();
    }

    updateBgMove();

    if (_musFade && !_musFade->done())
        _musFade->update();

    if (_elapsedFrames >= NEXT_SCENE_FRAMES)
    {
        reqStackClear();
        reqStackPush(SceneId::INTRO_LOGO);
    }

    return true;
}

bool IntroStory::isSkipRequested()
{
    return _skipCountdown >= 0;
}

void IntroStory::updateBgFade()
{
    if (!_bgFade.done())
    {
        // fade-in is updated on every 2 frames
        if (_isFadeOut || _elapsedFrames % 2 == 0)
            _bgFade.update();

        if (_bgFade.done() && _isFadeOut)
        {
            if (++_introBgIdx0 < INTRO_BG_COUNT)
                _bg = buildIntroBg(_introBgIdx0);
        }
    }

    if (_introBgIdx0 < INTRO_BG_COUNT)
        startNextBgFade();
}

void IntroStory::startNextBgFade()
{
    BN_ASSERT(0 <= _introBgIdx0 && _introBgIdx0 < INTRO_BG_COUNT);

    const auto& curBgFade = INTRO_BGS[_introBgIdx0];

    if (_elapsedFrames == curBgFade.fadeInStart)
    {
        _isFadeOut = false;
        const int fadeInFrames = (_introBgIdx0 == INTRO_BG_COUNT - 1) ? LAST_FADE_IN_FRAMES : FADE_IN_FRAMES;
        _bgFade = buildBgFadeIn(fadeInFrames);
    }
    else if (_elapsedFrames == curBgFade.fadeOutStart)
    {
        _isFadeOut = true;
        const bool isLastFadeOut = (_introBgIdx0 == INTRO_BG_COUNT - 1);
        const int fadeOutFrames = isLastFadeOut ? LAST_FADE_OUT_FRAMES : FADE_OUT_FRAMES;
        _bgFade = buildBgFadeOut(fadeOutFrames);
        if (isLastFadeOut)
            _musFade = buildMusFadeOut(fadeOutFrames);
    }
}

void IntroStory::updateBgMove()
{
    if (_elapsedFrames == BG_MOVE_START_FRAME)
        _bgMove = bn::regular_bg_move_to_action(_bg, BG_MOVE_FRAMES, LAST_BG_END_POS);

    if (_bgMove && !_bgMove->done())
        _bgMove->update();
}

} // namespace ut::scene
