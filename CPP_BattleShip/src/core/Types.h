/*****************************************************************************
+ Project: Battleship - Raylib Edition                                      +
 + File: Types.h                                                             +
 +                                                                           +
 + Shared data types used across multiple modules.                           +
 + No logic, no dependencies — pure data structures and enums.               +
 *****************************************************************************/

#pragma once

// Represents a grid coordinate
struct Coordinate
{
    int row    = 0;
    int column = 0;
};

enum class ShotResult : uint8_t
{
    HIT, 
    SUNK, 
    MISS
};