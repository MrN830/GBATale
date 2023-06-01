#include "debug/MemView.hpp"

#include <bn_bgs.h>
#include <bn_core.h>
#include <bn_display.h>
#include <bn_fixed_point.h>
#include <bn_format.h>
#include <bn_keypad.h>
#include <bn_memory.h>
#include <bn_sprites.h>

#include "core/TextGens.hpp"

namespace ut::debug
{

namespace
{

constexpr auto TOP_LEFT_ORIGIN = bn::fixed_point{-bn::display::width() / 2, -bn::display::height() / 2};

constexpr auto LINE_DIFF = bn::fixed_point{0, 6};

constexpr auto CPU_POS = bn::fixed_point{1, 3} + TOP_LEFT_ORIGIN;
constexpr auto BG_SPR_POS = CPU_POS + LINE_DIFF * 1;
constexpr auto IWRAM_POS = CPU_POS + LINE_DIFF * 2;
constexpr auto EWRAM_POS = CPU_POS + LINE_DIFF * 3;

} // namespace

MemView::MemView(core::TextGens& textGens) : _textGen(textGens.get(asset::FontKind::CRYPT))
{
    redrawTexts();
}

void MemView::update()
{
    if (bn::keypad::select_pressed())
        setVisible(!_isVisible);

    if (_isVisible && --_updateCountdown <= 0)
    {
        _updateCountdown = UPDATE_INTERVAL;
        redrawTexts();
    }
}

void MemView::setVisible(bool isVisible)
{
    if (isVisible == _isVisible)
        return;

    _isVisible = isVisible;
    _updateCountdown = UPDATE_INTERVAL;
    redrawTexts();
}

void MemView::redrawTexts()
{
    _texts.clear();

    if (_isVisible)
    {
        _textGen.generate(BG_SPR_POS,
                          bn::format<14>("BG/SPR {}/{}", bn::bgs::used_items_count(), bn::sprites::used_items_count()),
                          _texts);
        _textGen.generate(CPU_POS, bn::format<10>("CPU {}%", (bn::core::last_cpu_usage() * 100).round_integer()),
                          _texts);
        const int usedIw = bn::memory::used_stack_iwram() + bn::memory::used_static_iwram();
        _textGen.generate(IWRAM_POS,
                          bn::format<13>("IW {}% {}", (bn::fixed(usedIw) / IWRAM_BYTES * 100).round_integer(), usedIw),
                          _texts);
        const int usedEw = EWRAM_BYTES - bn::memory::available_alloc_ewram();
        _textGen.generate(EWRAM_POS,
                          bn::format<14>("EW {}% {}", (bn::fixed(usedEw) / EWRAM_BYTES * 100).round_integer(), usedEw),
                          _texts);

        for (auto& text : _texts)
            text.set_bg_priority(0);
    }
}

} // namespace ut::debug
