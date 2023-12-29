#pragma once

#include <bn_string.h>

#include "game/ItemInfo.hpp"

#include "consts.hpp"

namespace ut::core
{

struct DialogVariableInfo
{
    static constexpr int DIALOG_VAR_MAX_CHARS = 30;

    bn::string<consts::CHARA_NAME_MAX_LEN> charaName = "Chara";
    game::ItemKind item = game::ItemKind::NONE;
    int gold = 0;
    bn::string<DIALOG_VAR_MAX_CHARS> arg1, arg2;
};

} // namespace ut::core
