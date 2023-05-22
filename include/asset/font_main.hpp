#pragma once

#include <bn_sprite_font.h>
#include <bn_utf8_characters_map.h>

#include "bn_sprite_items_ft_main.h"

namespace ut::asset
{

inline constexpr bn::utf8_character font_main_utf8_characters[] = {"Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á",
                                                                   "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"};

inline constexpr int8_t font_main_character_widths[] = {
    4, // 32
    4, // 33 !
    6, // 34 "
    7, // 35 #
    6, // 36 $
    6, // 37 %
    8, // 38 &
    2, // 39 '
    4, // 40 (
    4, // 41 )
    7, // 42 *
    6, // 43 +
    2, // 44 ,
    6, // 45 -
    2, // 46 .
    6, // 47 /
    6, // 48 0
    6, // 49 1
    6, // 50 2
    6, // 51 3
    6, // 52 4
    6, // 53 5
    6, // 54 6
    6, // 55 7
    6, // 56 8
    6, // 57 9
    2, // 58 :
    2, // 59 ;
    5, // 60 <
    6, // 61 =
    5, // 62 >
    6, // 63 ?
    8, // 64 @
    6, // 65 A
    6, // 66 B
    6, // 67 C
    6, // 68 D
    6, // 69 E
    6, // 70 F
    6, // 71 G
    6, // 72 H
    6, // 73 I
    6, // 74 J
    6, // 75 K
    6, // 76 L
    7, // 77 M
    6, // 78 N
    6, // 79 O
    6, // 80 P
    6, // 81 Q
    6, // 82 R
    6, // 83 S
    6, // 84 T
    6, // 85 U
    6, // 86 V
    7, // 87 W
    6, // 88 X
    6, // 89 Y
    6, // 90 Z
    4, // 91 [
    6, // 92
    4, // 93 ]
    6, // 94 ^
    6, // 95 _
    3, // 96 `
    6, // 97 a
    6, // 98 b
    6, // 99 c
    6, // 100 d
    6, // 101 e
    6, // 102 f
    6, // 103 g
    6, // 104 h
    6, // 105 i
    6, // 106 j
    6, // 107 k
    6, // 108 l
    7, // 109 m
    6, // 110 n
    6, // 111 o
    6, // 112 p
    6, // 113 q
    6, // 114 r
    6, // 115 s
    6, // 116 t
    6, // 117 u
    6, // 118 v
    7, // 119 w
    6, // 120 x
    6, // 121 y
    6, // 122 z
    5, // 123 {
    2, // 124 |
    5, // 125 }
    6, // 126 ~
    8, // Á
    8, // É
    8, // Í
    8, // Ó
    8, // Ú
    8, // Ü
    8, // Ñ
    8, // á
    8, // é
    8, // í
    8, // ó
    8, // ú
    8, // ü
    8, // ñ
    8, // ¡
    8, // ¿
};

inline constexpr bn::span<const bn::utf8_character> font_main_utf8_characters_span(font_main_utf8_characters);

inline constexpr auto font_main_utf8_characters_map = bn::utf8_characters_map<font_main_utf8_characters_span>();

inline constexpr bn::sprite_font font_main(bn::sprite_items::ft_main, font_main_utf8_characters_map.reference(),
                                           font_main_character_widths, 1);

} // namespace ut::asset
