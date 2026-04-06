/*****************************************************************************
+ Project: Battleship - Raylib Edition                                       +
 + File: Colors.h                                                            +
 +                                                                           +
 + Defines all game colors as raylib Color-Structs.                           +
 *****************************************************************************/

#pragma once
#include "raylib.h"

namespace GameColors
{
    // ── Backgrounds ────────────────────
    inline constexpr Color BACKGROUND  = {  8,  10,  28, 255 };
    inline constexpr Color PANEL       = { 20,  22,  45, 255 };

    // ── Board cells ────────────────────
    inline constexpr Color CELL_EMPTY  = { 18,  22,  50, 255 };
    inline constexpr Color CELL_HOVER  = { 45,  70, 120, 255 };
    inline constexpr Color CELL_SHIP   = { 30, 180,  80, 255 };
    inline constexpr Color CELL_HIT    = {210,  50,  50, 255 };
    inline constexpr Color CELL_MISS   = { 40,  90, 200, 255 };
    inline constexpr Color GRID_LINE   = { 50,  55,  90, 255 };

    // ── Text colors ────────────────────
    inline constexpr Color TEXT_NORMAL  = { 220, 220, 240, 255 };
    inline constexpr Color TEXT_DIM     = { 110, 115, 155, 255 };
    inline constexpr Color TEXT_SUCCESS = {  60, 220, 100, 255 };
    inline constexpr Color TEXT_WARNING = { 240, 180,  40, 255 };
    inline constexpr Color TEXT_ERROR   = { 230,  60,  60, 255 };
    inline constexpr Color TEXT_ACCENT  = { 100, 200, 255, 255 };

    // ── Button colors ────────────────────
    inline constexpr Color BTN_NORMAL  = { 40,  50, 105, 255 };
    inline constexpr Color BTN_HOVER   = { 70,  90, 165, 255 };
    inline constexpr Color BTN_TEXT    = { 230, 235, 255, 255 };
}