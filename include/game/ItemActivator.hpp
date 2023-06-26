#pragma once

#include <bn_string.h>
#include <bn_string_view.h>
#include <bn_vector.h>

#include "consts.hpp"

namespace ut::core
{
class Random;
}

namespace ut::game
{

class GameState;

class ItemActivator
{
public:
    // instant noodles in battle => 17 lines
    static constexpr int USE_MAX_DIALOGS = 17;

public:
    /// @return dialog strings
    [[nodiscard]] static auto use(int itemIdx, GameState&, core::Random&)
        -> bn::vector<bn::string<consts::DIALOG_MAX_CHARS>, USE_MAX_DIALOGS>;
    /// @return dialog strings
    [[nodiscard]] static auto info(int itemIdx, GameState&) -> bn::vector<bn::string_view, 2>;
    /// @return dialog strings
    [[nodiscard]] static auto drop(int itemIdx, GameState&) -> bn::string<consts::DIALOG_MAX_CHARS>;
};

} // namespace ut::game
