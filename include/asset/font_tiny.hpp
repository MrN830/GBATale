#pragma once

#include <bn_sprite_font.h>
#include <bn_utf8_characters_map.h>

#include "bn_sprite_items_ft_tiny.h"

namespace ut::asset
{

inline constexpr bn::utf8_character font_tiny_utf8_characters[] = {"Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á",
                                                                   "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"};

inline constexpr int8_t font_tiny_character_widths[] = {
    2, // 32
    1, // 33 !
    3, // 34 "
    5, // 35 #
    3, // 36 $
    3, // 37 %
    5, // 38 &
    1, // 39 '
    2, // 40 (
    2, // 41 )
    3, // 42 *
    3, // 43 +
    1, // 44 ,
    3, // 45 -
    1, // 46 .
    3, // 47 /
    3, // 48 0
    3, // 49 1
    3, // 50 2
    3, // 51 3
    3, // 52 4
    3, // 53 5
    3, // 54 6
    3, // 55 7
    3, // 56 8
    3, // 57 9
    1, // 58 :
    1, // 59 ;
    2, // 60 <
    3, // 61 =
    2, // 62 >
    3, // 63 ?
    5, // 64 @
    3, // 65 A
    3, // 66 B
    3, // 67 C
    3, // 68 D
    3, // 69 E
    3, // 70 F
    3, // 71 G
    3, // 72 H
    3, // 73 I
    3, // 74 J
    3, // 75 K
    3, // 76 L
    5, // 77 M
    3, // 78 N
    3, // 79 O
    3, // 80 P
    3, // 81 Q
    3, // 82 R
    3, // 83 S
    3, // 84 T
    3, // 85 U
    3, // 86 V
    5, // 87 W
    3, // 88 X
    3, // 89 Y
    3, // 90 Z
    2, // 91 [
    3, // 92
    2, // 93 ]
    3, // 94 ^
    3, // 95 _
    1, // 96 `
    3, // 97 a
    3, // 98 b
    3, // 99 c
    3, // 100 d
    3, // 101 e
    3, // 102 f
    3, // 103 g
    3, // 104 h
    1, // 105 i
    2, // 106 j
    3, // 107 k
    2, // 108 l
    5, // 109 m
    3, // 110 n
    3, // 111 o
    3, // 112 p
    3, // 113 q
    3, // 114 r
    3, // 115 s
    3, // 116 t
    3, // 117 u
    3, // 118 v
    5, // 119 w
    3, // 120 x
    3, // 121 y
    3, // 122 z
    3, // 123 {
    1, // 124 |
    3, // 125 }
    1, // 126 ~
    1, // Á
    1, // É
    1, // Í
    1, // Ó
    1, // Ú
    1, // Ü
    1, // Ñ
    1, // á
    1, // é
    1, // í
    1, // ó
    1, // ú
    1, // ü
    1, // ñ
    1, // ¡
    1, // ¿
};

inline constexpr bn::span<const bn::utf8_character> font_tiny_utf8_characters_span(font_tiny_utf8_characters);

inline constexpr auto font_tiny_utf8_characters_map = bn::utf8_characters_map<font_tiny_utf8_characters_span>();

inline constexpr bn::sprite_font font_tiny(bn::sprite_items::ft_tiny, font_tiny_utf8_characters_map.reference(),
                                           font_tiny_character_widths, 1);

} // namespace ut::asset
