#pragma once

#include <bn_sprite_font.h>
#include <bn_utf8_characters_map.h>

#include "bn_sprite_items_ft_dmg_nums.h"

namespace ut::asset
{

inline constexpr bn::utf8_character font_dmg_nums_utf8_characters[] = {"Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á",
                                                                       "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"};

inline constexpr int8_t font_dmg_nums_character_widths[] = {
    8,  // 32
    1,  // 33 !
    1,  // 34 "
    1,  // 35 #
    1,  // 36 $
    1,  // 37 %
    1,  // 38 &
    1,  // 39 '
    1,  // 40 (
    1,  // 41 )
    1,  // 42 *
    1,  // 43 +
    1,  // 44 ,
    1,  // 45 -
    1,  // 46 .
    1,  // 47 /
    16, // 48 0
    16, // 49 1
    16, // 50 2
    16, // 51 3
    16, // 52 4
    16, // 53 5
    16, // 54 6
    16, // 55 7
    16, // 56 8
    16, // 57 9
    1,  // 58 :
    1,  // 59 ;
    1,  // 60 <
    1,  // 61 =
    1,  // 62 >
    1,  // 63 ?
    1,  // 64 @
    1,  // 65 A
    1,  // 66 B
    1,  // 67 C
    1,  // 68 D
    1,  // 69 E
    1,  // 70 F
    1,  // 71 G
    1,  // 72 H
    12, // 73 I
    1,  // 74 J
    1,  // 75 K
    1,  // 76 L
    16, // 77 M
    1,  // 78 N
    1,  // 79 O
    1,  // 80 P
    1,  // 81 Q
    1,  // 82 R
    16, // 83 S
    1,  // 84 T
    1,  // 85 U
    1,  // 86 V
    1,  // 87 W
    1,  // 88 X
    1,  // 89 Y
    1,  // 90 Z
    1,  // 91 [
    1,  // 92
    1,  // 93 ]
    1,  // 94 ^
    1,  // 95 _
    1,  // 96 `
    1,  // 97 a
    1,  // 98 b
    1,  // 99 c
    1,  // 100 d
    1,  // 101 e
    1,  // 102 f
    1,  // 103 g
    1,  // 104 h
    1,  // 105 i
    1,  // 106 j
    1,  // 107 k
    1,  // 108 l
    1,  // 109 m
    1,  // 110 n
    1,  // 111 o
    1,  // 112 p
    1,  // 113 q
    1,  // 114 r
    1,  // 115 s
    1,  // 116 t
    1,  // 117 u
    1,  // 118 v
    1,  // 119 w
    1,  // 120 x
    1,  // 121 y
    1,  // 122 z
    1,  // 123 {
    1,  // 124 |
    1,  // 125 }
    1,  // 126 ~
    1,  // Á
    1,  // É
    1,  // Í
    1,  // Ó
    1,  // Ú
    1,  // Ü
    1,  // Ñ
    1,  // á
    1,  // é
    1,  // í
    1,  // ó
    1,  // ú
    1,  // ü
    1,  // ñ
    1,  // ¡
    1,  // ¿
};

inline constexpr bn::span<const bn::utf8_character> font_dmg_nums_utf8_characters_span(font_dmg_nums_utf8_characters);

inline constexpr auto font_dmg_nums_utf8_characters_map = bn::utf8_characters_map<font_dmg_nums_utf8_characters_span>();

inline constexpr bn::sprite_font font_dmg_nums(bn::sprite_items::ft_dmg_nums,
                                               font_dmg_nums_utf8_characters_map.reference(),
                                               font_dmg_nums_character_widths, 0);

} // namespace ut::asset
