#pragma once

#include <cstdint>

namespace bn
{
class string_view;
}

namespace ut::asset::gen
{

enum class TextData : int16_t;

auto getTextEn(TextData) -> const bn::string_view&;

} // namespace ut::asset::gen
