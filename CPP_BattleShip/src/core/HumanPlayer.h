/*****************************************************************************
 + Project: Battleship - Raylib Edition                                      +
 + File: HumanPlayer.h                                                       +
 +                                                                           +
 + Human player controlled by mouse input.                                   +
 +                                                                           +
 + Kept to maintain the Player hierarchy and allow future                    +
 + extensions such as local multiplayer.                                     +
 *****************************************************************************/

#pragma once

#include "Player.h"

class HumanPlayer : public Player
{
public:
    // ── Constructor / Destructor ────────────────────
    explicit HumanPlayer(int boardSize);
    ~HumanPlayer() override = default;
    
    Coordinate MakeMove() override { return {}; }

    // HumanPlayer has no additional logic in the graphical version.
    // Kept to maintain the Player hierarchy and allow future extensions
    // (e.g. local multiplayer).
};