#pragma once

#include <bn_sprite_font.h>
#include <bn_utf8_characters_map.h>

#include "bn_sprite_items_ft_crypt.h"

namespace ut::asset
{

inline constexpr bn::utf8_character font_crypt_utf8_characters[] = {"Á", "É", "Í", "Ó", "Ú", "Ü", "Ñ", "á",
                                                                    "é", "í", "ó", "ú", "ü", "ñ", "¡", "¿"};

inline constexpr int8_t font_crypt_character_widths[] = {
    2, // 32
    2, // 33 !
    3, // 34 "
    4, // 35 #
    4, // 36 $
    5, // 37 %
    4, // 38 &
    2, // 39 '
    3, // 40 (
    3, // 41 )
    4, // 42 *
    3, // 43 +
    2, // 44 ,
    4, // 45 -
    2, // 46 .
    4, // 47 /
    4, // 48 0
    4, // 49 1
    4, // 50 2
    4, // 51 3
    4, // 52 4
    4, // 53 5
    4, // 54 6
    4, // 55 7
    4, // 56 8
    4, // 57 9
    2, // 58 :
    2, // 59 ;
    4, // 60 <
    4, // 61 =
    4, // 62 >
    4, // 63 ?
    4, // 64 @
    4, // 65 A
    4, // 66 B
    4, // 67 C
    4, // 68 D
    4, // 69 E
    4, // 70 F
    4, // 71 G
    4, // 72 H
    4, // 73 I
    4, // 74 J
    4, // 75 K
    4, // 76 L
    4, // 77 M
    4, // 78 N
    4, // 79 O
    4, // 80 P
    4, // 81 Q
    4, // 82 R
    4, // 83 S
    4, // 84 T
    4, // 85 U
    4, // 86 V
    4, // 87 W
    4, // 88 X
    4, // 89 Y
    4, // 90 Z
    3, // 91 [
    4, // 92
    3, // 93 ]
    4, // 94 ^
    5, // 95 _
    2, // 96 `
    4, // 97 a
    4, // 98 b
    4, // 99 c
    4, // 100 d
    4, // 101 e
    4, // 102 f
    4, // 103 g
    4, // 104 h
    4, // 105 i
    4, // 106 j
    4, // 107 k
    4, // 108 l
    4, // 109 m
    4, // 110 n
    4, // 111 o
    4, // 112 p
    4, // 113 q
    4, // 114 r
    4, // 115 s
    4, // 116 t
    4, // 117 u
    4, // 118 v
    4, // 119 w
    4, // 120 x
    4, // 121 y
    4, // 122 z
    4, // 123 {
    4, // 124 |
    4, // 125 }
    4, // 126 ~
    4, // Á
    4, // É
    4, // Í
    4, // Ó
    4, // Ú
    4, // Ü
    4, // Ñ
    4, // á
    4, // é
    4, // í
    4, // ó
    4, // ú
    4, // ü
    4, // ñ
    4, // ¡
    4, // ¿
};

inline constexpr bn::span<const bn::utf8_character> font_crypt_utf8_characters_span(font_crypt_utf8_characters);

inline constexpr auto font_crypt_utf8_characters_map = bn::utf8_characters_map<font_crypt_utf8_characters_span>();

inline constexpr bn::sprite_font font_crypt(bn::sprite_items::ft_crypt, font_crypt_utf8_characters_map.reference(),
                                            font_crypt_character_widths, 1);

} // namespace ut::asset
