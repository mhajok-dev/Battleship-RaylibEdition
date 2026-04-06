/*****************************************************************************
+ Project: Battleship - Raylib Edition                                      +
 + File: Settings.h                                                          +
 +                                                                           +
 + Central place for all compile-time game constants.                        +
 + No logic, no dependencies — pure configuration values.                   +
 *****************************************************************************/

#pragma once

namespace GameSettings
{
    // ── Window ────────────────────
    constexpr int WINDOW_WIDTH  = 1280;
    constexpr int WINDOW_HEIGHT = 720;
    constexpr int TARGET_FPS    = 60;

    // ── Board ────────────────────
    constexpr int MIN_BOARD_SIZE = 5;
    constexpr int MAX_BOARD_SIZE = 15;

    // ── Timing ────────────────────
    constexpr float AI_DELAY     = 1.5f;
    constexpr float SWITCH_DELAY = 1.5f;
}